%code requires {
	#include "../inc/treeNode.h"
    #include <bits/stdc++.h>
}
%define parse.error verbose

%{
    #include <bits/stdc++.h>
	#include "../inc/treeNode.h"
    #include "../inc/symbolTable.h" 
    #include "../inc/mipsGenerator.h"
    #include <sys/stat.h> // for mkdir

    extern CodeGenerator codeGen;
    using namespace std;
    void yyerror(const char *s);
    
    // Forward declaration for MIPS generation
    void generateMIPSFromTAC(const string& outputFilename);

    extern int yylex();
    extern int yylineno;
    extern FILE *yyin;
    extern unordered_set<string> classOrStructOrUnion;
    extern unordered_set<string> typedefNames;
    extern int expectedReturnType;
    map<string,string> Label_defn;
    // Map to store forward goto references: label_name -> list of instruction indices to backpatch
    map<string, vector<int>> forwardGotoRefs;
    
    void registerTypedefName(const string& name) {
        typedefNames.insert(name);
    }
    
    // Register typedef names from declarator list immediately
    void registerTypedefNamesFromDeclaration(TreeNode* declSpecifiers, TreeNode* declaratorList) {
        // Check if this is a typedef declaration
        bool isTypedef = false;
        for (auto child : declSpecifiers->children) {
            if (child->type == NODE_STORAGE_CLASS_SPECIFIER && child->valueToString() == "typedef") {
                isTypedef = true;
                break;
            }
        }
        
        if (!isTypedef) return;
        
        // Extract names from declarator list and register them
        for (auto declarator : declaratorList->children) {
            if (declarator->type != NODE_DECLARATOR || declarator->children.empty()) continue;
            
            TreeNode* firstChild = declarator->children[0];
            if (!firstChild) continue;
            
            // Navigate to the identifier node
            TreeNode* identNode = firstChild;
            while (identNode) {
                if (identNode->type == NODE_IDENTIFIER) {
                    registerTypedefName(identNode->valueToString());
                    break;
                } else if (identNode->type == NODE_POINTER || 
                           identNode->type == NODE_REFERENCE ||
                           identNode->type == ARRAY) {
                    if (!identNode->children.empty()) {
                        identNode = identNode->children[0];
                    } else {
                        break;
                    }
                } else if (identNode->type == NODE_PARAMETER_LIST) {
                    // This is a function declarator, skip
                    break;
                } else if (!identNode->children.empty()) {
                    identNode = identNode->children[0];
                } else {
                    break;
                }
            }
        }
    }
    
    void backTrackExpr(TreeNode* nd){
        if(nd == nullptr) return;
        if(!nd->trueList.empty() || !nd->falseList.empty()){
            // Backpatch trueList to jump to the "result = 1" assignment
            Backpatch::backpatch(nd->trueList, to_string(codeGen.currentInstrIndex));
            // Backpatch falseList to jump to the "result = 0" assignment
            Backpatch::backpatch(nd->falseList, to_string(codeGen.currentInstrIndex + 2));
            // True case: set result = 1, then jump past the false case
            codeGen.emit(TACOp::ASSIGN, nd->tacResult, "1", nullopt);
            codeGen.emit(TACOp::GOTO, to_string(codeGen.currentInstrIndex + 1), nullopt, nullopt);
            // False case: set result = 0
            codeGen.emit(TACOp::ASSIGN, nd->tacResult, "0", nullopt);
        }
    }

void backTrackRelExpr(TreeNode* nd){
        if(nd == nullptr) return;
        // For a relational expression like "a != b", the comparison instruction
        // (NE, EQ, LT, etc.) with isGoto=true has ALREADY been emitted by the caller.
        // We need to:
        // 1. Add the comparison instruction index (currentInstrIndex - 1) to trueList
        // 2. Emit an unconditional goto for the false case
        // 3. Add this goto index to falseList
        // 
        // The trueList contains the comparison instruction (jumps when TRUE)
        // The falseList contains the unconditional goto (jumps when FALSE)
        int comparisonIndex = codeGen.currentInstrIndex - 1;  // The already-emitted NE instruction
        nd->trueList.push_back(comparisonIndex);
        int falseGotoIndex = codeGen.currentInstrIndex;  // Save index before emit
        codeGen.emit(TACOp::oth, "", nullopt, nullopt, true);  // Emit unconditional goto for false case
        nd->falseList.push_back(falseGotoIndex);  // The just-emitted goto
}

TACOp getTACOpFromAssignment(const string &assignOp, bool isBooleanContext) {
    if (assignOp == "=") return isBooleanContext ? TACOp::oth : TACOp::ASSIGN;
    if (assignOp == "+=") return TACOp::ADD;
    if (assignOp == "-=") return TACOp::SUB;
    if (assignOp == "*=") return TACOp::MUL;
    if (assignOp == "/=") return TACOp::DIV;
    if (assignOp == "%=") return TACOp::MOD;
    if (assignOp == "&=") return TACOp::BIT_AND;
    if (assignOp == "|=") return TACOp::BIT_OR;
    if (assignOp == "^=") return TACOp::BIT_XOR;
    if (assignOp == "<<=") return TACOp::LSHFT;
    if (assignOp == ">>=") return TACOp::RSHFT;
    throw invalid_argument("Unsupported assignment operator: " + assignOp);
}

void handleAssignment(const string &assignOp, const string &lhs, const string &rhs, bool isBooleanContext = false) {
    TACOp op = getTACOpFromAssignment(assignOp, isBooleanContext);
    if (assignOp == "=" && isBooleanContext) {
        codeGen.emit(op, lhs, rhs, nullopt);
    } else {
        codeGen.emit(op, lhs, lhs, rhs);
    }
}

// Helper function to extract array name and all indices from nested array access
// e.g., arr[i][j] returns ("arr", ["i", "j"])
pair<string, vector<string>> extractArrayIndices(TreeNode* node) {
    vector<string> indices;
    TreeNode* current = node;
    
    // Traverse from the outermost index inward
    while (current && current->isArrayIndex && current->children.size() >= 2) {
        indices.insert(indices.begin(), current->children[1]->tacResult);
        current = current->children[0];
    }
    
    // current now points to the base array
    string arrayName = current ? current->tacResult : "";
    return {arrayName, indices};
}

// Flatten multi-dimensional array indices based on array dimensions
// For arr[2][3] accessed as arr[i][j]: flat_index = i * 3 + j
string flattenArrayIndex(const string& arrayName, const vector<string>& indices, const vector<int>& dimensions) {
    if (indices.empty()) return "";
    if (indices.size() == 1) return indices[0];  // 1D array, no flattening needed
    
    // For multi-dimensional arrays, compute: ((i * D2 + j) * D3 + k) * D4 ...
    string result = indices[0];
    
    for (size_t i = 1; i < indices.size(); i++) {
        // Multiply previous result by next dimension
        if (i < dimensions.size()) {
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::MUL, temp, result, to_string(dimensions[i]));
            result = temp;
        }
        
        // Add current index
        string temp2 = codeGen.newTemp();
        codeGen.emit(TACOp::ADD, temp2, result, indices[i]);
        result = temp2;
    }
    
    return result;
}

// Emit INDEX operation for array access if not already emitted (for rvalue use)
void emitIndexIfNeeded(TreeNode* node) {
    if (node && node->isArrayIndex && node->children.size() == 2) {
        // Extract all indices from nested array access
        auto [arrayName, indices] = extractArrayIndices(node);
        
        // Look up array dimensions from symbol table
        TreeNode* arrayNode = lookupSymbol(arrayName);
        vector<int> dimensions;
        if (arrayNode && arrayNode->typeCategory == 2) {
            dimensions = arrayNode->dimensions;
        }
        
        // Handle struct member arrays (e.g., g.matrix)
        // Parse struct.member notation
        if (dimensions.empty() && arrayName.find('.') != string::npos) {
            // Extract struct and member names
            size_t dotPos = arrayName.find('.');
            string structName = arrayName.substr(0, dotPos);
            string memberName = arrayName.substr(dotPos + 1);
// Function support enhanced
