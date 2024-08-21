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
// Control flow structures added
