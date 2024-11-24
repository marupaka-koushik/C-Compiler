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
            
            // Look up struct definition
            TreeNode* structNode = lookupSymbol(structName);
            if (structNode && structNode->symbolTable.size() > 0) {
                // Find the member in the struct's symbol table
                for (const auto& [name, memberNode] : structNode->symbolTable) {
                    if (name == memberName && memberNode->typeCategory == 2) {
                        dimensions = memberNode->dimensions;
                        break;
                    }
                }
            }
        }
        
        // Flatten the indices if multi-dimensional
        string flatIndex;
        if (indices.size() > 1 && dimensions.size() > 1) {
            flatIndex = flattenArrayIndex(arrayName, indices, dimensions);
        } else {
            flatIndex = indices.empty() ? "0" : indices[0];
        }
        
        // Emit INDEX with flattened index
        string temp = codeGen.newTemp();
        codeGen.emit(TACOp::INDEX, temp, arrayName, flatIndex);
        node->tacResult = temp;
    }
}

%}


%union {
	TreeNode *node;
    char *str;
    int integer;
    int intVal;
}


%token <node>
    KEYWORD_AUTO KEYWORD_BOOL KEYWORD_BREAK KEYWORD_CASE KEYWORD_CHAR 
    KEYWORD_CLASS KEYWORD_CONST KEYWORD_CONTINUE KEYWORD_DEFAULT KEYWORD_DELETE KEYWORD_DO 
    KEYWORD_DOUBLE KEYWORD_ELSE KEYWORD_ENUM KEYWORD_EXTERN KEYWORD_FLOAT 
    KEYWORD_FOR KEYWORD_GOTO KEYWORD_IF KEYWORD_INT 
    KEYWORD_LONG KEYWORD_NEW KEYWORD_NULLPTR KEYWORD_PRIVATE KEYWORD_PROTECTED 
    KEYWORD_PUBLIC KEYWORD_REGISTER KEYWORD_RETURN KEYWORD_SHORT KEYWORD_SIGNED KEYWORD_SIZEOF 
    KEYWORD_STATIC KEYWORD_STRUCT KEYWORD_SWITCH KEYWORD_THIS KEYWORD_UNION
    KEYWORD_TYPEDEF KEYWORD_UNSIGNED 
    KEYWORD_VOID KEYWORD_VOLATILE KEYWORD_WHILE KEYWORD_UNTIL KEYWORD_PRINTF KEYWORD_SCANF TYPE_NAME TYPEDEF_NAME

%token <node> INTEGER FLOAT CHAR STRING ID ELLIPSIS_OPERATOR BOOLEAN_LITERAL


%token <str>
    LBRACE RBRACE LBRACKET RBRACKET LPAREN RPAREN SEMICOLON COMMA COLON


%token <str>
    PLUS_OPERATOR MINUS_OPERATOR MULTIPLY_OPERATOR DIVIDE_OPERATOR MODULO_OPERATOR
    DECREMENT_OPERATOR INCREMENT_OPERATOR


%token <str>
    ASSIGNMENT_OPERATOR PLUS_ASSIGN_OPERATOR MINUS_ASSIGN_OPERATOR MULTIPLY_ASSIGN_OPERATOR 
    DIVIDE_ASSIGN_OPERATOR MODULO_ASSIGN_OPERATOR BITWISE_AND_ASSIGN_OPERATOR BITWISE_OR_ASSIGN_OPERATOR 
    BITWISE_XOR_ASSIGN_OPERATOR RIGHT_SHIFT_ASSIGN_OPERATOR LEFT_SHIFT_ASSIGN_OPERATOR


%token <str>
    EQUALS_COMPARISON_OPERATOR NOT_EQUALS_OPERATOR GREATER_THAN_OPERATOR LESS_THAN_OPERATOR 
    GREATER_THAN_OR_EQUAL_OPERATOR LESS_THAN_OR_EQUAL_OPERATOR


%token <str>
    LOGICAL_AND_OPERATOR LOGICAL_OR_OPERATOR LOGICAL_NOT_OPERATOR


%token <str>
    BITWISE_AND_OPERATOR BITWISE_OR_OPERATOR BITWISE_XOR_OPERATOR LEFT_SHIFT_OPERATOR 
    RIGHT_SHIFT_OPERATOR BITWISE_NOT_OPERATOR 


%token <str>
    TERNARY_OPERATOR DOT_OPERATOR  SCOPE_RESOLUTION_OPERATOR  
    POINTER_TO_MEMBER_DOT_OPERATOR POINTER_TO_MEMBER_ARROW_OPERATOR

%type<node> translation_unit external_declaration function_definition constructor_function destructor_function struct_type_specifier

%type<node> declaration declaration_specifiers declarator compound_statement struct_declaration_list M N  

%type<node> storage_class_specifier type_specifier struct_or_union_specifier struct_or_union enum_specifier enumerator_list enumerator class_specifier member_declaration_list member_declaration access_specifier

%type<node> struct_declaration struct_declarator_list specifier_qualifier_list type_qualifier constant_expression

%type<node> type_qualifier_list parameter_type_list parameter_list parameter_declaration identifier_list type_name abstract_declarator

%type<node> initializer initializer_list direct_declarator pointer reference direct_abstract_declarator assignment_expression 

%type<node> statement labeled_statement expression_statement selection_statement iteration_statement jump_statement block_item block_item_list

%type<node> expression init_declarator init_declarator_list conditional_expression primary_expression postfix_expression for_cond for_inc for_init

%type<node> unary_expression unary_operator cast_expression multiplicative_expression additive_expression shift_expression

%type<node> relational_expression equality_expression and_expression exclusive_or_expression inclusive_or_expression scope_resolution_statements

%type<node> logical_and_expression logical_or_expression argument_expression_list assignment_operator io_statement scope_resolution_statement single_expression

%start translation_unit

%nonassoc NO_ELSE
%nonassoc KEYWORD_ELSE
%%

N
: {
    $$ = new TreeNode(OTHERS);
    $$->nextList.push_back(codeGen.currentInstrIndex);
    codeGen.emit(TACOp::oth, "", nullopt, nullopt, true);
}

primary_expression
	: ID { 
        $$ = $1; 
        TreeNode* looked = lookupSymbol($$->valueToString());
        
        // If not found, keep the original ID node
        if (looked == nullptr) {
            $$->tacResult = $1->valueToString();
            $$->isLValue = true;
        }
        // Handle enum constants
        else if (looked->isConstant) {
            $$ = looked;
            // Treat enum constant as an integer literal
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::ASSIGN, temp, to_string($$->constantValue), nullopt);
            $$->tacResult = temp;
            $$->isLValue = false;
        } else {
            $$ = looked;
            $$->tacResult = $1->valueToString();
            $$->isLValue = true;
        }
    }
	| INTEGER { 
        $$ = $1;
        string temp = codeGen.newTemp();
        codeGen.emit(TACOp::ASSIGN, temp, $1->valueToString(), nullopt);
        $$->tacResult = temp;
        $$->typeSpecifier = 3; 
        $$->isLValue = false;
    }
    | FLOAT { 
        $$ = $1;
        string temp = codeGen.newTemp();
        codeGen.emit(TACOp::ASSIGN, temp, $1->valueToString(), nullopt);
        $$->tacResult = temp;
        $$->typeSpecifier = 6; 
        $$->isLValue = false; 
    }
	| STRING { 
        $$ = $1;
        string temp = codeGen.newTemp();
        codeGen.emit(TACOp::ASSIGN, temp, $1->valueToString(), nullopt);
        $$->tacResult = temp; 
        $$->typeSpecifier = 8; 
        $$->isLValue = false; 
    }
	| CHAR { 
        $$ = $1; 
        string temp = codeGen.newTemp();
        codeGen.emit(TACOp::ASSIGN, temp, $1->valueToString(), nullopt);
        $$->tacResult = temp;
        $$->typeSpecifier = 1; 
        $$->isLValue = false; 
    }
    | BOOLEAN_LITERAL { 
        $$ = $1;
        $$->typeSpecifier=5;
        string temp = codeGen.newTemp();
        $$->tacResult = temp;
        if($1->valueToString() == "true"){
            $$->trueList.push_back(codeGen.currentInstrIndex);
        }else{
            $$->falseList.push_back(codeGen.currentInstrIndex);
        }
    } 
    | KEYWORD_NULLPTR { 
        $$ = $1;
        $$->tacResult = "nullptr";
        $$->typeSpecifier = 9; 
        $$->isLValue = false; 
    }
    | KEYWORD_THIS { 
        $$ = $1;
        $$->tacResult = "this";
        $$->isLValue = false;
    }
	| LPAREN expression RPAREN { 
        $$ = $2;
    }
	;

postfix_expression
    : primary_expression { 
        $$ = $1; 
    }
    | postfix_expression LBRACKET expression RBRACKET { 
        $$ = createNode(NODE_POSTFIX_EXPRESSION, monostate(), $1, $3);
        if ($3->typeSpecifier != 3) {
            cerr << "Error: array index must be an integer" << endl;
        } else if ($1->typeCategory == 2) {
            $$->typeSpecifier = $1->typeSpecifier;
            $$->isLValue = true;
            $$->isArrayIndex = true;  // Mark this as array index expression
            
            // CRITICAL: Handle multi-dimensional arrays
            // Copy dimensions from parent and remove the first one (we're indexing into it)
            if (!$1->dimensions.empty()) {
                $$->dimensions = vector<int>($1->dimensions.begin() + 1, $1->dimensions.end());
                // If still have dimensions left, result is still an array
                if (!$$->dimensions.empty()) {
                    $$->typeCategory = 2;  // Still an array
                } else {
                    $$->typeCategory = 0;  // Now a scalar value
                }
            } else {
                $$->typeCategory = 0;
                $$->pointerLevel = 0;
            }
            
            // Check if this is an array of pointers (e.g., int *arr[10])
            if ($1->pointerLevel > 0) {
                $$->typeCategory = 1;  // Result is a pointer
                $$->pointerLevel = $1->pointerLevel;
            }
            
            // Copy symbol table for array of structs
            if ($1->typeSpecifier == 20 && $1->symbolTable.size() > 0) {
                $$->symbolTable = $1->symbolTable;
            }
            
            // For lvalue (assignment target), use array notation: arr[index]
            // For rvalue (reading), emitIndexIfNeeded will emit INDEX instruction
            // Store array notation in tacResult for proper member access and assignment
            $$->tacResult = $1->tacResult + "[" + $3->tacResult + "]";
        } else if ($1->typeCategory == 1 || $1->pointerLevel > 0) {
            // Pointer indexing (including multi-level pointers like char**)
            $$->typeSpecifier = $1->typeSpecifier;
            $$->isLValue = true;
            $$->isArrayIndex = true;  // Mark this as array/pointer index expression
            
            // If indexing a pointer to pointer (e.g., argv[0] where argv is char**)
            // The result should be a pointer (e.g., char*)
            if ($1->pointerLevel > 1) {
                $$->typeCategory = 1;  // Still a pointer
                $$->pointerLevel = $1->pointerLevel - 1;
            } else if ($1->pointerLevel == 1) {
                $$->typeCategory = 0;  // Now a value
                $$->pointerLevel = 0;
            } else {
                $$->typeCategory = 0;
                $$->pointerLevel = 0;
            }
            
            // For lvalue (assignment target), use array notation: ptr[index]
            // For rvalue (reading), emitIndexIfNeeded will emit INDEX instruction
            // Store array notation in tacResult for proper member access and assignment
            $$->tacResult = $1->tacResult + "[" + $3->tacResult + "]";
        } else {
            cerr << $1->valueToString() << " is not an array" << endl;
        }
    }
    | postfix_expression LPAREN RPAREN { 
        $$ = $1;
        if ($$->paramCount > 0) {
            cerr << "Error: function call with no params, expected " << $$->paramCount << endl;
            YYABORT;
        }else{
            if($1->typeSpecifier == 0){
                codeGen.emit(TACOp::CALL2, "", $1->tacResult , "0");
            }else{
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::CALL, temp, $1->tacResult, "0");
                $$->tacResult = temp;
            }
            $$->pointerLevel = $1->pointerLevel; // Copy pointer level from function signature
            $$->isLValue = false;
        } 
    }
    | postfix_expression LPAREN argument_expression_list RPAREN { 
        $$ = createNode(NODE_POSTFIX_EXPRESSION, monostate(), $1, $3); 
        $$->typeSpecifier = $1->typeSpecifier;
        $$->pointerLevel = $1->pointerLevel; // Copy pointer level from function signature
        $$->isLValue = false;
        if ($1->typeCategory == 3) {
            if ($1->paramCount == $3->children.size()) {
                for (int i = 0; i < $1->paramCount; i++) {
                    int lhs = $1->paramTypes[i];
                    int rhs = $3->children[i]->typeSpecifier;
                    if (!isTypeCompatible(lhs, rhs, "=")) {
                        cerr << "Expected: " << $1->paramTypes[i] << ", Got: " << $3->children[i]->typeSpecifier << endl;
                        break;
                    }else{
                        // Only emit typecast if types differ AND it's not a pointer compatibility cast
                        if(lhs != rhs){
                            string castType = typeCastInfo(lhs, rhs);
                            // Skip typecast for pointer conversions (ptr_cast) - they're compatible as-is
                            if (castType != "ptr_cast") {
                                string temp = codeGen.newTemp();
                                codeGen.emit(TACOp::TYPECAST, temp, castType, $3->children[i]->tacResult);
                                $3->children[i]->tacResult = temp;
                            }
                        }
                    }
                }
            } else {
                cerr << "Error: function call with " << $3->children.size() << " params, expected " << $1->paramCount << endl;
                YYABORT;
            }
        }
        for (auto* arg : $3->children) {
            emitIndexIfNeeded(arg);  // Emit INDEX if argument is array access
            codeGen.emit(TACOp::ASSIGN, "param", arg->tacResult, nullopt);
        }
        // Functions with void type AND no pointer level don't return a value (true void)
        // But void* (typeSpec=0, ptrLevel=1) does return a value
        if($1->typeSpecifier == 0 && $1->pointerLevel == 0){
            codeGen.emit(TACOp::CALL2, "", $1->valueToString(), to_string($3->children.size()));
        }else{
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::CALL, temp, $1->valueToString(), to_string($3->children.size()));
            $$->tacResult = temp; 
        }
    }
    | postfix_expression DOT_OPERATOR ID { 
        $$ = createNode(NODE_POSTFIX_EXPRESSION, $2, $1, $3);
        
        // Check if using . on a pointer (should use -> instead)
        if ($1->pointerLevel > 0) {
            cerr << "Error: dot operator '.' cannot be used on pointers; use '->' instead" << endl;
            YYABORT;
        }
        
        if ($1->typeSpecifier == 20) {
            // $1 is a struct (could be a variable like 'emp' or a nested member like 'emp.joinDate')
            TreeNode* structInfo = nullptr;
            
            // Check if $1 already has a symbolTable (nested member access)
            if ($1->symbolTable.size() > 0) {
                // This is a nested access like emp.joinDate.day
                // $1 already has the symbol table of the struct type
                structInfo = $1;
            } else {
                // This is a first-level access like emp.id
                // Look up the variable to get its symbol table
                structInfo = lookupSymbol($1->tacResult);
            }
            
            if (structInfo != nullptr && structInfo->symbolTable.size() > 0) {
                // Now look for the member in the struct's symbol table
                bool found = false;
                int offset = 0;
                for (auto entry : structInfo->symbolTable) {
                    if (entry.first == $3->valueToString()) {
                        // Found the member
                        found = true;
                        $$->typeSpecifier = entry.second->typeSpecifier;
                        $$->typeCategory = entry.second->typeCategory;
                        $$->pointerLevel = entry.second->pointerLevel;
                        $$->storageClass = entry.second->storageClass;
                        $$->isConst = entry.second->isConst;
                        $$->isStatic = entry.second->isStatic;
                        $$->isVolatile = entry.second->isVolatile;
                        $$->isUnsigned = entry.second->isUnsigned;
                        $$->isLValue = true;
                        
                        // Copy the symbol table if this member is also a struct
                        if (entry.second->typeSpecifier == 20) {
                            $$->symbolTable = entry.second->symbolTable;
                        }
                        
                        // Copy dimensions if this member is an array
                        if (entry.second->typeCategory == 2) {
                            $$->dimensions = entry.second->dimensions;
                        }
                        
                        // Generate member access: structVar.member
                        $$->tacResult = $1->tacResult + "." + $3->valueToString();
                        
                        // Propagate basePointer for (*ptr).member chains
                        if (!$1->basePointer.empty()) {
                            $$->basePointer = $1->basePointer;
                        }
                        break;
                    } else {
                        // Calculate offset for this member (for future offset-based access)
                        switch (entry.second->typeSpecifier) {
                            case 1: offset += 1; break; // char
                            case 2: offset += 2; break; // short  
                            case 3: offset += 4; break; // int
                            case 4: offset += 8; break; // long
                            case 6: offset += 4; break; // float
                            case 7: offset += 8; break; // double
                            default: offset += 4; break;
                        }
                    }
                }
                if (!found) {
                    cerr << "Error: member '" << $3->valueToString() << "' not found in struct" << endl;
                    YYABORT;
                }
            } else {
                cerr << "Error: variable '" << $1->tacResult << "' not found or is not a struct" << endl;
                YYABORT;
            }
        } else {
            cerr << "Error: member access operator '.' can only be used with structs, classes, or unions" << endl;
            YYABORT;
        }
    }
    | postfix_expression POINTER_TO_MEMBER_ARROW_OPERATOR ID { 
        $$ = createNode(NODE_POSTFIX_EXPRESSION, $2, $1, $3);
        // The arrow operator is for pointer-to-struct: ptr->member is equivalent to (*ptr).member
        // $1 should be a pointer to a struct (typeSpecifier 20, pointerLevel > 0)
        if ($1->pointerLevel > 0 && $1->typeSpecifier == 20) {
            // Look up the pointer variable to get the struct type's symbol table
            TreeNode* ptrInfo = nullptr;
            if ($1->symbolTable.size() > 0) {
                // Already has symbol table (nested access)
                ptrInfo = $1;
            } else {
                // Look up the pointer variable
                ptrInfo = lookupSymbol($1->tacResult);
            }
            
            if (ptrInfo != nullptr && ptrInfo->symbolTable.size() > 0) {
                // Find the member in the struct's symbol table
                bool found = false;
                for (auto entry : ptrInfo->symbolTable) {
                    if (entry.first == $3->valueToString()) {
                        found = true;
                        $$->typeSpecifier = entry.second->typeSpecifier;
                        $$->typeCategory = entry.second->typeCategory;
                        $$->pointerLevel = entry.second->pointerLevel;
                        $$->isLValue = true;
                        
                        // Copy symbol table if this member is also a struct
                        if (entry.second->typeSpecifier == 20) {
                            $$->symbolTable = entry.second->symbolTable;
                        }
                        
                        // Generate proper 3AC for pointer member access
                        $$->tacResult = $1->tacResult + "->" + $3->valueToString();
                        break;
                    }
                }
                if (!found) {
                    cerr << "Error: member '" << $3->valueToString() << "' not found in pointed-to struct" << endl;
                    YYABORT;
                }
            } else {
                cerr << "Error: '" << $1->tacResult << "' is not a pointer to struct" << endl;
                YYABORT;
            }
        } else {
            cerr << "Error: arrow operator '->' requires a pointer to struct" << endl;
            YYABORT;
        }
    }
    | postfix_expression POINTER_TO_MEMBER_DOT_OPERATOR ID { 
        $$ = createNode(NODE_POSTFIX_EXPRESSION, $2, $1, $3);
        $$->isLValue = true;
        string temp = codeGen.newTemp();
        string memberAccess = $1->tacResult + ".*" + $3->valueToString();
        codeGen.emit(TACOp::ASSIGN, temp, memberAccess, nullopt);
        $$->tacResult = temp;
    }
    | postfix_expression INCREMENT_OPERATOR { 
        if (!$1->isLValue) {  
            cerr << "Error: Cannot post-increment an R-value at line " << yylineno << endl;
        }
        $$ = $1;
        $$->type = NODE_POSTFIX_EXPRESSION;
        int typeSpec = $1->typeSpecifier;
        if (typeSpec == 5 || typeSpec > 7) {
            cerr << "Error: invalid type for increment operator" << endl;
        }
        $$->isLValue = false; 
        string temp = codeGen.newTemp();
        codeGen.emit(TACOp::ASSIGN, temp, $1->tacResult, nullopt);
        string temp2 = codeGen.newTemp();
        codeGen.emit(TACOp::ADD, temp2, $1->tacResult, "1");
        codeGen.emit(TACOp::ASSIGN, $1->tacResult, temp2, nullopt);
        $$->tacResult = temp;
    }
    | postfix_expression DECREMENT_OPERATOR {
        if (!$1->isLValue) {
            cerr << "Error: Cannot post-decrement an R-value at line " << yylineno << endl;
        } 
        $$ = $1;
        $$->type = NODE_POSTFIX_EXPRESSION;
        int typeSpec = $1->typeSpecifier;
        if (typeSpec == 5 || typeSpec > 7) {
            cerr << "Error: invalid type for decrement operator" << endl;
        }
        $$->isLValue = false;
        string temp = codeGen.newTemp();
        codeGen.emit(TACOp::ASSIGN, temp, $1->tacResult, nullopt);
        string temp2 = codeGen.newTemp();
        codeGen.emit(TACOp::SUB, temp2, $1->tacResult, "1");
        codeGen.emit(TACOp::ASSIGN, $1->tacResult, temp2, nullopt);
        $$->tacResult = temp;
    }
    ;

argument_expression_list
    : assignment_expression { 
        $$ = createNode(NODE_ARGUMENT_EXPRESSION_LIST, monostate(), $1); 
    }
    | argument_expression_list COMMA assignment_expression { 
        $$ = $1;
        $$->children.push_back($3);
    }
    ;

unary_expression
    : postfix_expression { 
        $$ = $1; 
        $$->type = NODE_UNARY_EXPRESSION;
        $$->isLValue = $1->isLValue;
        $$->isArrayIndex = $1->isArrayIndex;  // Preserve array index flag
    }
    | INCREMENT_OPERATOR unary_expression {
        if (!$2->isLValue) {  
            cerr << "Error: Cannot pre-increment an R-value at line " << yylineno << endl;
        } 
        $$ = $2;
        codeGen.emit(TACOp::ADD, $2->tacResult, $2->tacResult, "1");
        $$->tacResult = $2->tacResult;
        int typeSpec = $2->typeSpecifier;
        if (typeSpec == 5 || typeSpec > 7) {
            cerr << "Error: invalid type for increment operator" << endl;
        }
        $$->isLValue = false; 
    }
    | DECREMENT_OPERATOR unary_expression {
        if (!$2->isLValue) {
            cerr << "Error: Cannot pre-decrement an R-value at line " << yylineno << endl;
        }
        $$ = $2;
        codeGen.emit(TACOp::SUB, $2->tacResult, $2->tacResult, "1");
        $$->tacResult = $2->tacResult;
        int typeSpec = $2->typeSpecifier;
        $$->isLValue = false; 
        if (typeSpec == 5 || typeSpec > 7) {
            cerr << "Error: invalid type for decrement operator" << endl;
        }
    }
    | KEYWORD_SIZEOF LPAREN type_name RPAREN {
        // sizeof(type) - return size of type in bytes
        $$ = createNode(NODE_UNARY_EXPRESSION, monostate());
        $$->typeSpecifier = 3;  // int
        $$->isLValue = false;
        
        // Extract type information from type_name
        TreeNode* typeNode = $3;
        int typeSpec = 3;  // Default to int
        int ptrLevel = 0;
        
        // type_name can be just specifier_qualifier_list or specifier_qualifier_list + abstract_declarator
        if (typeNode->type == NODE_SPECIFIER_QUALIFIER_LIST) {
            // Direct specifier_qualifier_list
            if (!typeNode->children.empty() && typeNode->children[0]->type == NODE_TYPE_SPECIFIER) {
                typeSpec = typeNode->children[0]->storageClass;
            }
        } else if (typeNode->type == NODE_TYPE_NAME && !typeNode->children.empty()) {
            // type_name with children (specifier_qualifier_list + optional abstract_declarator)
            TreeNode* firstChild = typeNode->children[0];
            
            if (firstChild->type == NODE_SPECIFIER_QUALIFIER_LIST && !firstChild->children.empty()) {
                TreeNode* typeSpecNode = firstChild->children[0];
                if (typeSpecNode->type == NODE_TYPE_SPECIFIER) {
                    typeSpec = typeSpecNode->storageClass;
                }
            }
            
            // Check for pointer in abstract declarator
            if (typeNode->children.size() > 1) {
                TreeNode* absDecl = typeNode->children[1];
                // abstract_declarator can be:
                // 1. Just a pointer: abstract_declarator -> pointer
                // 2. Wrapped pointer: abstract_declarator -> NODE_ABSTRACT_DECLARATOR -> pointer
                if (absDecl->type == NODE_POINTER) {
                    // Direct pointer (e.g., sizeof(char*))
                    ptrLevel = 1;
                } else if (absDecl->type == NODE_ABSTRACT_DECLARATOR && !absDecl->children.empty()) {
                    if (absDecl->children[0]->type == NODE_POINTER) {
                        ptrLevel = 1;
                    }
                }
            }
        }
        
        int size = 4;  // Default size
        if (ptrLevel > 0) {
            size = 8;  // Pointer size (64-bit)
        } else {
            switch(typeSpec) {
                case 0: size = 0; break;  // void
                case 1: size = 1; break;  // char
                case 2: size = 2; break;  // short
                case 3: size = 4; break;  // int
                case 4: size = 4; break;  // long (32-bit)
                case 12: size = 4; break;  // float
                case 13: size = 8; break;  // double
                case 7: size = 1; break;  // bool
                default: size = 4; break;
            }
        }
        
        $$->tacResult = to_string(size);
    }
    | KEYWORD_SIZEOF unary_expression {
        // sizeof(expression) - return size of expression's type
        $$ = createNode(NODE_UNARY_EXPRESSION, monostate());
        $$->typeSpecifier = 3;  // int
        $$->isLValue = false;
        
        int typeSpec = $2->typeSpecifier;
        int size = 4;  // Default size for element
        
        switch(typeSpec) {
            case 1: size = 1; break;  // char
            case 2: size = 2; break;  // short
            case 3: size = 4; break;  // int
            case 4: size = 4; break;  // long
            case 5: size = 4; break;  // float
            case 6: size = 8; break;  // double
            case 7: size = 1; break;  // bool
            default: size = 4; break;
        }
        
        if ($2->pointerLevel > 0) {
            size = 8;  // Pointer size
        }
        // Check if it's an array
        else if ($2->typeCategory == 2 && !$2->dimensions.empty()) {
            // Calculate total array size: element_size * product of all dimensions
            int totalElements = 1;
            for (int dim : $2->dimensions) {
                if (dim > 0) {
                    totalElements *= dim;
                }
            }
            size = size * totalElements;
        }
        
        $$->tacResult = to_string(size);
    }
    | unary_operator cast_expression { 
        $$ = createNode(NODE_UNARY_EXPRESSION, monostate(), $1, $2);
        $$->typeSpecifier = $2->typeSpecifier;
        string temp = codeGen.newTemp();
        string op = $1->valueToString();
        if (op == "&") {
            // Check if operand is an lvalue
            if (!$2->isLValue) {
                cerr << "Error: Cannot take address of non-lvalue (e.g., literal or rvalue)" << endl;
                YYABORT;
            }
            
            // If taking address of a multi-dimensional array access, flatten the indices first
            string operand = $2->tacResult;
            if ($2->isArrayIndex && $2->children.size() == 2) {
                auto [arrayName, indices] = extractArrayIndices($2);
                
                // Look up array dimensions
                TreeNode* arrayNode = lookupSymbol(arrayName);
                vector<int> dimensions;
                if (arrayNode && arrayNode->typeCategory == 2) {
                    dimensions = arrayNode->dimensions;
                }
                
                // Handle struct member arrays (e.g., g.matrix)
                if (dimensions.empty() && arrayName.find('.') != string::npos) {
                    size_t dotPos = arrayName.find('.');
                    string structName = arrayName.substr(0, dotPos);
                    string memberName = arrayName.substr(dotPos + 1);
                    
                    TreeNode* structNode = lookupSymbol(structName);
                    if (structNode && structNode->symbolTable.size() > 0) {
                        for (const auto& [name, memberNode] : structNode->symbolTable) {
                            if (name == memberName && memberNode->typeCategory == 2) {
                                dimensions = memberNode->dimensions;
                                break;
                            }
                        }
                    }
                }
                
                // Flatten indices if multi-dimensional
                if (indices.size() > 1 && dimensions.size() > 1) {
                    string flatIndex = flattenArrayIndex(arrayName, indices, dimensions);
                    operand = arrayName + "[" + flatIndex + "]";
                } else if (!indices.empty()) {
                    operand = arrayName + "[" + indices[0] + "]";
                }
            }
            
            codeGen.emit(TACOp::ASSIGN, temp, "&" + operand, nullopt);
            $$->isLValue = false;
            $$->pointerLevel = $2->pointerLevel + 1;
            $$->typeCategory = 1;  // Result is a pointer
        } else if (op == "*") {
            // Check if operand is a pointer
            if ($2->pointerLevel <= 0) {
                cerr << "Error: Cannot dereference non-pointer type (pointer level: " << $2->pointerLevel << ")" << endl;
                YYABORT;
            }
            codeGen.emit(TACOp::ASSIGN, temp, "*" + $2->tacResult, nullopt);
            $$->isLValue = true;
            $$->pointerLevel = $2->pointerLevel - 1;
            // Set typeCategory based on resulting pointer level
            if ($$->pointerLevel > 0) {
                $$->typeCategory = 1;  // Still a pointer
            } else {
                $$->typeCategory = 0;  // Now a value
            }
            
            // If dereferencing a pointer to struct, copy the symbol table
            // so that member access (*ptr).member can work
            // Also store the original pointer for assignment tracking
            if ($2->typeSpecifier == 20 && $2->symbolTable.size() > 0) {
                $$->symbolTable = $2->symbolTable;
                $$->basePointer = $2->tacResult;  // Store original pointer variable
            }
        } else if (op == "+") {
            codeGen.emit(TACOp::ASSIGN, temp, $2->tacResult, nullopt);
            $$->isLValue = false;
        } else if (op == "-") {
            codeGen.emit(TACOp::SUB, temp, "0", $2->tacResult);
            $$->isLValue = false;
        } else if (op == "~") {
            codeGen.emit(TACOp::BIT_XOR, temp, $2->tacResult, "-1");
            $$->falseList = $2->falseList;
            $$->trueList = $2->trueList;
            $$->isLValue = false;
        } else if (op == "!") {
            // Logical NOT: DON'T swap lists, just invert the comparison operator
            // The comparison should jump when the NOT condition is TRUE
            // For !(a == b), we want "if a != b goto TRUE_BLOCK"
            $$->trueList = $2->trueList;  // Keep trueList as-is
            $$->falseList = $2->falseList;  // Keep falseList as-is
            
            // Invert ONLY the comparison operators in trueList (the conditional jumps)
            for (int idx : $$->trueList) {
                if (idx >= 0 && idx < codeGen.tacCode.size()) {
                    TACInstruction& instr = codeGen.tacCode[idx];
                    // Invert the comparison operator
                    switch (instr.op) {
                        case TACOp::EQ: instr.op = TACOp::NE; break;
                        case TACOp::NE: instr.op = TACOp::EQ; break;
                        case TACOp::LT: instr.op = TACOp::GE; break;
                        case TACOp::GE: instr.op = TACOp::LT; break;
                        case TACOp::GT: instr.op = TACOp::LE; break;
                        case TACOp::LE: instr.op = TACOp::GT; break;
                        default: break;  // Not a comparison, leave as-is (e.g., unconditional goto)
                    }
                }
            }
            // Don't invert falseList - it contains unconditional gotos which shouldn't change
            
            // Only emit comparison if operand doesn't have control flow lists
            if ($2->trueList.empty() && $2->falseList.empty()) {
                codeGen.emit(TACOp::EQ, "", $2->tacResult, "0", true);  // Use "" for result, set isGoto=true
                backTrackRelExpr($$);
            } else {
                // Operand is a boolean expression with control flow
                // Just use its tacResult (may be empty for pure control flow)
                temp = $2->tacResult;
            }
            $$->isLValue = false;
        }
        $$->tacResult = temp;         
        if (op == "&" && !$2->isLValue) {
            cerr << "Error: Cannot apply '&' to an R-value at line " << yylineno << endl;
        } else if (op == "*" && $2->pointerLevel == 0) {
            cerr << "Error: Cannot dereference a non-pointer type at line " << yylineno << endl;
        }
    }
    ;



unary_operator
	: BITWISE_AND_OPERATOR { $$ = createNode(NODE_UNARY_OPERATOR, $1); }
	| MULTIPLY_OPERATOR { $$ = createNode(NODE_UNARY_OPERATOR, $1); }
	| PLUS_OPERATOR { $$ = createNode(NODE_UNARY_OPERATOR, $1); }
	| MINUS_OPERATOR { $$ = createNode(NODE_UNARY_OPERATOR, $1); }
	| BITWISE_NOT_OPERATOR { $$ = createNode(NODE_UNARY_OPERATOR, $1); }
	| LOGICAL_NOT_OPERATOR { $$ = createNode(NODE_UNARY_OPERATOR,$1); }
	;

cast_expression
	: unary_expression { $$ = $1; }
	| LPAREN type_name RPAREN cast_expression
        {
            $$ = createNode(NODE_CAST_EXPRESSION, monostate(), $2, $4);
            
            // Extract type information from type_name
            int typeSpec = 3;  // Default to int
            int ptrLevel = 0;
            
            // First, try to get typeSpec from the type_name node itself
            if ($2->typeSpecifier != -1) {
                typeSpec = $2->typeSpecifier;
            }
            
            // type_name can be just specifier_qualifier_list or specifier_qualifier_list + abstract_declarator
            TreeNode* typeNode = $2;
            if (typeNode->type == NODE_TYPE_NAME && !typeNode->children.empty()) {
                // type_name with children (specifier_qualifier_list + optional abstract_declarator)
                TreeNode* firstChild = typeNode->children[0];
                
                if (firstChild->type == NODE_SPECIFIER_QUALIFIER_LIST && !firstChild->children.empty()) {
                    TreeNode* typeSpecNode = firstChild->children[0];
                    if (typeSpecNode->type == NODE_TYPE_SPECIFIER) {
                        if (typeSpecNode->typeSpecifier != -1) {
                            typeSpec = typeSpecNode->typeSpecifier;
                        } else if (typeSpecNode->storageClass != 0) {
                            typeSpec = typeSpecNode->storageClass;
                        } else {
                            // Try to look up as typedef name
                            string typeName = typeSpecNode->valueToString();
                            TreeNode* typedefDef = lookupSymbol(typeName);
                            if (typedefDef && typedefDef->typeSpecifier != -1) {
                                typeSpec = typedefDef->typeSpecifier;
                            }
                        }
                    }
                }
                
                // Check for pointer in abstract declarator
                if (typeNode->children.size() > 1) {
                    TreeNode* absDecl = typeNode->children[1];
                    TreeNode* curr = absDecl;
                    while (curr && (curr->type == NODE_POINTER || curr->type == NODE_ABSTRACT_DECLARATOR)) {
                        if (curr->type == NODE_POINTER) {
                            ptrLevel++;
                            if (!curr->children.empty()) {
                                curr = curr->children[0];
                            } else {
                                break;
                            }
                        } else if (!curr->children.empty()) {
                            curr = curr->children[0];
                        } else {
                            break;
                        }
                    }
                }
            } else if (typeNode->type == NODE_SPECIFIER_QUALIFIER_LIST && !typeNode->children.empty()) {
                // Direct specifier_qualifier_list
                TreeNode* typeSpecNode = typeNode->children[0];
                if (typeSpecNode->type == NODE_TYPE_SPECIFIER) {
                    if (typeSpecNode->typeSpecifier != -1) {
                        typeSpec = typeSpecNode->typeSpecifier;
                    } else if (typeSpecNode->storageClass != 0) {
                        typeSpec = typeSpecNode->storageClass;
                    } else {
                        // Try to look up as typedef name
                        string typeName = typeSpecNode->valueToString();
                        TreeNode* typedefDef = lookupSymbol(typeName);
                        if (typedefDef && typedefDef->typeSpecifier != -1) {
                            typeSpec = typedefDef->typeSpecifier;
                        }
                    }
                }
            }
            
            int toType = typeSpec;
            int fromType = $4->typeSpecifier;

            if (!isValidCast(toType, fromType)) {
               cerr <<  "Invalid type cast" << endl;
            }
            
            $$->typeSpecifier = typeSpec;
            $$->pointerLevel = ptrLevel;
            $$->isLValue = false;
            
            // For cast expressions, the result is just the casted expression's tacResult
            // Don't create a string representation - just pass through the value
            $$->tacResult = $4->tacResult;
        }
	;

multiplicative_expression
	: cast_expression { 
        $$ = $1; 
    }
	| multiplicative_expression MULTIPLY_OPERATOR cast_expression { 
        $$ = createNode(NODE_MULTIPLICATIVE_EXPRESSION, $2, $1, $3);
        backTrackExpr($1);
        backTrackExpr($3);
        emitIndexIfNeeded($1);
        emitIndexIfNeeded($3);
        $$->isLValue = false; 
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(rhsPointerLevel || lhsPointerLevel){
                cerr << "Incompatible types: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
        if (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, "*")) {
            $$->typeSpecifier = max($1->typeSpecifier, $3->typeSpecifier);
            if($1->typeSpecifier != $3->typeSpecifier){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                $3->tacResult = temp;
            }
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::MUL, temp, $1->tacResult, $3->tacResult);
            $$->tacResult = temp;
        } else {
            // Type compatibility check
        }
        
        }
    }
	| multiplicative_expression DIVIDE_OPERATOR cast_expression { 
        $$ = createNode(NODE_MULTIPLICATIVE_EXPRESSION, $2, $1, $3);
        $$->isLValue = false;
        backTrackExpr($1);
        backTrackExpr($3);
        emitIndexIfNeeded($1);
        emitIndexIfNeeded($3);
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(rhsPointerLevel||lhsPointerLevel){
                cerr << "Incompatible types: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
        if (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, "/")) {
            if($1->typeSpecifier != $3->typeSpecifier){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                $3->tacResult = temp;
            }
            $$->typeSpecifier = max($1->typeSpecifier, $3->typeSpecifier);
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::DIV, temp, $1->tacResult, $3->tacResult);
            $$->tacResult = temp;
        } else {
            // Type compatibility check
        }
        }
    }
	| multiplicative_expression MODULO_OPERATOR cast_expression { 
        $$ = createNode(NODE_MULTIPLICATIVE_EXPRESSION, $2, $1, $3);
        backTrackExpr($1);
        backTrackExpr($3);
        emitIndexIfNeeded($1);
        emitIndexIfNeeded($3);
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(rhsPointerLevel || lhsPointerLevel){
                cerr << "Incompatible types: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
        $$->isLValue = false; 
        if (($1->typeSpecifier == 3 || $1->typeSpecifier == 4) && 
            ($3->typeSpecifier == 3 || $3->typeSpecifier == 4) && 
            isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, "%")) {
                if($1->typeSpecifier != $3->typeSpecifier){
                    string temp = codeGen.newTemp();
                    codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                    $3->tacResult = temp;
                }
                $$->typeSpecifier = max($1->typeSpecifier, $3->typeSpecifier);
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::MOD, temp, $1->tacResult, $3->tacResult);
                $$->tacResult = temp;
        } else {
            // Type compatibility check
        }
    }
    }
	;

additive_expression
	: multiplicative_expression { 
        $$ = $1; 

    }
	| additive_expression PLUS_OPERATOR multiplicative_expression {
        $$ = createNode(NODE_ADDITIVE_EXPRESSION, $2, $1, $3);
        backTrackExpr($1);
        backTrackExpr($3);
        emitIndexIfNeeded($1);  // Emit INDEX if operand is array access
        emitIndexIfNeeded($3);
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(rhsPointerLevel && lhsPointerLevel){
                cerr << "Error: Cannot add two pointers (invalid pointer arithmetic) at line " << yylineno << endl;
                YYABORT;
        }
        else{
    $$->isLValue = false; 
    $$->pointerLevel = lhsPointerLevel + rhsPointerLevel;
    
    // Handle pointer arithmetic: pointer + integer or integer + pointer
    bool isPointerArithmetic = (rhsPointerLevel > 0 && lhsPointerLevel == 0) || 
                               (rhsPointerLevel == 0 && lhsPointerLevel > 0);
    
    if (isPointerArithmetic) {
        // Result type is the pointer's type
        $$->typeSpecifier = (rhsPointerLevel > 0) ? $1->typeSpecifier : $3->typeSpecifier;
        $$->typeCategory = 0;
        
        // Emit ADD operation (MIPS generator will handle pointer scaling)
        string temp = codeGen.newTemp();
        codeGen.emit(TACOp::ADD, temp, $1->tacResult, $3->tacResult);
        $$->tacResult = temp;
    } else if (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, "+")) {
        $$->typeSpecifier = max($1->typeSpecifier, $3->typeSpecifier);
        if($1->typeSpecifier != $3->typeSpecifier){
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
            $3->tacResult = temp;
        }
        string temp = codeGen.newTemp(); 
        if ($1->typeCategory == 2 || $3->typeCategory == 2) {
            $$->typeCategory = 2;
            codeGen.emit(TACOp::ADD, temp, $1->tacResult, $3->tacResult);
        } else {
            $$->typeCategory = 0;
            codeGen.emit(TACOp::ADD, temp, $1->tacResult, $3->tacResult);
        }
        $$->tacResult = temp;
    } else {
        // Type compatibility check failed
        cerr << "Error: Incompatible types in addition at line " << yylineno << endl;
        YYABORT;
    } }

    }
	| additive_expression MINUS_OPERATOR multiplicative_expression { 
        $$ = createNode(NODE_ADDITIVE_EXPRESSION, $2, $1, $3);
        backTrackExpr($1);
        backTrackExpr($3);
        emitIndexIfNeeded($1);
        emitIndexIfNeeded($3);
        $$->isLValue = false;
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(rhsPointerLevel && lhsPointerLevel){
                cerr << "Incompatible types in compound assignment: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
        $$->pointerLevel = lhsPointerLevel + rhsPointerLevel;
        
        // Handle pointer arithmetic: pointer - integer
        bool isPointerArithmetic = (rhsPointerLevel > 0 && lhsPointerLevel == 0);
        
        if (isPointerArithmetic) {
            // Result type is the pointer's type
            $$->typeSpecifier = $1->typeSpecifier;
            $$->typeCategory = 0;
            
            // Emit SUB operation (MIPS generator will handle pointer scaling)
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::SUB, temp, $1->tacResult, $3->tacResult);
            $$->tacResult = temp;
        } else if (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, "-")) {
            if($1->typeSpecifier != $3->typeSpecifier){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                $3->tacResult = temp;
            }
            $$->typeSpecifier = max($1->typeSpecifier, $3->typeSpecifier);
            if($1->typeCategory==2 || $3->typeCategory==2){
                $$->typeCategory = 2;
            }
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::SUB, temp, $1->tacResult, $3->tacResult);
            $$->tacResult = temp; 
        } else {
            // Type compatibility check failed
            cerr << "Error: Incompatible types in subtraction at line " << yylineno << endl;
            YYABORT;
        } 
    }
    }
	;

shift_expression
	: additive_expression { 
        $$ = $1; 
    }
	| shift_expression LEFT_SHIFT_OPERATOR additive_expression { 
        $$ = createNode(NODE_MULTIPLICATIVE_EXPRESSION, $2, $1, $3);
        backTrackExpr($1);
        backTrackExpr($3);
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(rhsPointerLevel||lhsPointerLevel){
                cerr << "Incompatible types: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
        $$->isLValue = false; 
        if (($1->typeSpecifier == 3 || $1->typeSpecifier == 4) && 
            ($3->typeSpecifier == 3 || $3->typeSpecifier == 4) && 
            (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, "<<"))) {
            if($1->typeSpecifier != $3->typeSpecifier){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                $3->tacResult = temp;
            }
            $$->typeSpecifier = max($1->typeSpecifier, $3->typeSpecifier);
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::LSHFT, temp, $1->tacResult, $3->tacResult);
            $$->tacResult = temp;
        } else {
            // Type compatibility check
        }
        }
    }
	| shift_expression RIGHT_SHIFT_OPERATOR additive_expression {
        $$ = createNode(NODE_MULTIPLICATIVE_EXPRESSION, $2, $1, $3);
        $$->isLValue = false;
        backTrackExpr($1);
        backTrackExpr($3);
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(rhsPointerLevel||lhsPointerLevel){
                cerr << "Incompatible types: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
        if (($1->typeSpecifier == 3 || $1->typeSpecifier == 4) && 
            ($3->typeSpecifier == 3 || $3->typeSpecifier == 4) && 
            (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, ">>"))) {
            $$->typeSpecifier = max($1->typeSpecifier, $3->typeSpecifier);
            if($1->typeSpecifier != $3->typeSpecifier){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                $3->tacResult = temp;
            }
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::RSHFT, temp, $1->tacResult, $3->tacResult);
            $$->tacResult = temp; 
        } else {
            // Type compatibility check
        }
        }
    }
	;
    
relational_expression
	: shift_expression { $$ = $1; }
	| relational_expression LESS_THAN_OPERATOR shift_expression {
        $$ = createNode(NODE_RELATIONAL_EXPRESSION, $2, $1, $3);
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(rhsPointerLevel||lhsPointerLevel){
                cerr << "Incompatible types: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
        $$->isLValue = false; 
        if (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, "<")) {
            $$->typeSpecifier = 3;
            if($1->typeSpecifier != $3->typeSpecifier){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                $3->tacResult = temp;
            }
            $$->tacResult = codeGen.newTemp();
            codeGen.emit(TACOp::LT, "", $1->tacResult, $3->tacResult, true);
            backTrackRelExpr($$);
        } else {
            // Type compatibility check
        } 
    }
    }
	| relational_expression GREATER_THAN_OPERATOR shift_expression { 
        $$ = createNode(NODE_RELATIONAL_EXPRESSION, $2, $1, $3);
        $$->isLValue = false;
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(rhsPointerLevel||lhsPointerLevel){
                cerr << "Incompatible types: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
        if (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, ">")) {
            $$->typeSpecifier = 3;
            if($1->typeSpecifier != $3->typeSpecifier){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                $3->tacResult = temp;
            }
            codeGen.emit(TACOp::GT, "", $1->tacResult, $3->tacResult, true);
            backTrackRelExpr($$);  // CRITICAL: Call AFTER emit
            $$->tacResult = codeGen.newTemp();
        } else {
            // Type compatibility check
        } 
    }
    }
	| relational_expression LESS_THAN_OR_EQUAL_OPERATOR shift_expression {
        $$ = createNode(NODE_RELATIONAL_EXPRESSION, $2, $1, $3);
        $$->isLValue = false;
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(rhsPointerLevel||lhsPointerLevel){
                cerr << "Incompatible types: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
        if (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, "<=")) {
            $$->typeSpecifier = 3;
            if($1->typeSpecifier != $3->typeSpecifier){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                $3->tacResult = temp;
            }
            codeGen.emit(TACOp::LE, "", $1->tacResult, $3->tacResult, true);
            backTrackRelExpr($$);  // CRITICAL: Call AFTER emit
            $$->tacResult = codeGen.newTemp();
        } else {
            // Type compatibility check
        }
    }
    }
	| relational_expression GREATER_THAN_OR_EQUAL_OPERATOR shift_expression { 
        $$ = createNode(NODE_RELATIONAL_EXPRESSION, $2, $1, $3);
        $$->isLValue = false;
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(rhsPointerLevel||lhsPointerLevel){
                cerr << "Incompatible types: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
        if (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, ">=")) {
            $$->typeSpecifier = 3;
            if($1->typeSpecifier != $3->typeSpecifier){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, "",typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                $3->tacResult = temp;
            }
            codeGen.emit(TACOp::GE, "", $1->tacResult, $3->tacResult, true);
            backTrackRelExpr($$);  // CRITICAL: Call AFTER emit
            $$->tacResult = codeGen.newTemp();
        } else {
            // Type compatibility check
        } 
    }
    }
	;

equality_expression
	: relational_expression { $$ = $1; }
	| equality_expression EQUALS_COMPARISON_OPERATOR relational_expression { 
        $$ = createNode(NODE_EQUALITY_EXPRESSION, $2, $1, $3);
        $$->isLValue = false;
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        
        // Allow pointer-NULL comparison (pointer vs integer 0)
        bool isPointerNullComparison = (rhsPointerLevel > 0 && lhsPointerLevel == 0 && $3->typeSpecifier == 3) ||
                                       (lhsPointerLevel > 0 && rhsPointerLevel == 0 && $1->typeSpecifier == 3);
        
        if(rhsPointerLevel!=lhsPointerLevel && !isPointerNullComparison){
                cerr << "Incompatible types: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
        if (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, "==") || isPointerNullComparison) {
            $$->typeSpecifier = 3;
            // Emit INDEX for array accesses before comparison
            emitIndexIfNeeded($1);
            emitIndexIfNeeded($3);
            if($1->typeSpecifier != $3->typeSpecifier && !isPointerNullComparison){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                $3->tacResult = temp;
            }
            codeGen.emit(TACOp::EQ, "", $1->tacResult, $3->tacResult, true);
            $$->tacResult = codeGen.newTemp();
            backTrackRelExpr($$);  // CRITICAL: Call AFTER emit, not before!        } else {
            // Type compatibility check
        } 
    }
    }
	| equality_expression NOT_EQUALS_OPERATOR relational_expression { 
        $$ = createNode(NODE_EQUALITY_EXPRESSION, $2, $1, $3);
        $$->isLValue = false;
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        
        // Allow pointer-NULL comparison (pointer vs integer 0)
        bool isPointerNullComparison = (rhsPointerLevel > 0 && lhsPointerLevel == 0 && $3->typeSpecifier == 3) ||
                                       (lhsPointerLevel > 0 && rhsPointerLevel == 0 && $1->typeSpecifier == 3);
        
        if(rhsPointerLevel!=lhsPointerLevel && !isPointerNullComparison){
                cerr << "Incompatible types: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
        if (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, "!=") || isPointerNullComparison) {
            $$->typeSpecifier = 3;
            // Emit INDEX for array accesses before comparison
            emitIndexIfNeeded($1);
            emitIndexIfNeeded($3);
            if($1->typeSpecifier != $3->typeSpecifier && !isPointerNullComparison){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                $3->tacResult = temp;
            }
            codeGen.emit(TACOp::NE, "", $1->tacResult, $3->tacResult, true);
            // Call backTrackRelExpr AFTER emitting the NE instruction
            backTrackRelExpr($$);
            // Don't emit oth - backTrackRelExpr will emit it
            $$->tacResult = codeGen.newTemp();
        } else {
            // Type compatibility check
        }
        }
    }
	;


and_expression
	: equality_expression { $$ = $1; }
	| and_expression BITWISE_AND_OPERATOR equality_expression { 
        $$ = createNode(NODE_AND_EXPRESSION, $2, $1, $3); 
        $$->isLValue = false;
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(rhsPointerLevel || lhsPointerLevel){
                cerr << "Incompatible types: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
        if (($1->typeSpecifier == 3 || $1->typeSpecifier == 4) &&
            ($3->typeSpecifier == 3 || $3->typeSpecifier == 4) &&
            (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, "&"))) {
            $$->typeSpecifier = max($1->typeSpecifier, $3->typeSpecifier);
            if($1->typeSpecifier != $3->typeSpecifier){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                $3->tacResult = temp;
            }
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::BIT_AND, temp, $1->tacResult, $3->tacResult);
            $$->tacResult = temp;
        } else {
            // Type compatibility check
        }
    }
    }
	;

exclusive_or_expression
	: and_expression { $$ = $1; }
	| exclusive_or_expression BITWISE_XOR_OPERATOR and_expression { 
                int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(rhsPointerLevel || lhsPointerLevel){
                cerr << "Incompatible types: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
        $$ = createNode(NODE_EXCLUSIVE_OR_EXPRESSION, $2, $1, $3);
        $$->isLValue = false;
        if (($1->typeSpecifier == 3 || $1->typeSpecifier == 4) &&
            ($3->typeSpecifier == 3 || $3->typeSpecifier == 4) &&
            (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, "^"))) {
            $$->typeSpecifier = max($1->typeSpecifier, $3->typeSpecifier);
            if($1->typeSpecifier != $3->typeSpecifier){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                $3->tacResult = temp;
            }
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::BIT_XOR, temp, $1->tacResult, $3->tacResult);
            $$->tacResult = temp;
        } else {
            // Type compatibility check
        }
    }
    }
	;

inclusive_or_expression
	: exclusive_or_expression { $$ = $1; }
	| inclusive_or_expression BITWISE_OR_OPERATOR exclusive_or_expression {
                int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(rhsPointerLevel || lhsPointerLevel){
                cerr << "Incompatible types: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
        $$ = createNode(NODE_INCLUSIVE_OR_EXPRESSION, $2, $1, $3);
        $$->isLValue = false;
        if (($1->typeSpecifier == 3 || $1->typeSpecifier == 4) &&
            ($3->typeSpecifier == 3 || $3->typeSpecifier == 4) &&
            (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, "|"))) {
            $$->typeSpecifier = max($1->typeSpecifier, $3->typeSpecifier);
            if($1->typeSpecifier != $3->typeSpecifier){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                $3->tacResult = temp;
            }
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::BIT_OR, temp, $1->tacResult, $3->tacResult);
            $$->tacResult = temp;
        } else {
            // Type compatibility check
        }
        }
    }
	;

logical_and_expression
	: inclusive_or_expression { $$ = $1; }
	| logical_and_expression LOGICAL_AND_OPERATOR M inclusive_or_expression { 
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $4->pointerLevel;
        $$ = createNode(NODE_LOGICAL_AND_EXPRESSION, $2, $1, $4); 
        $$->isLValue = false;
        if (isTypeCompatible($1->typeSpecifier, $4->typeSpecifier, "&&")) {
            $$->typeSpecifier = 3;
            if($1->typeSpecifier != $4->typeSpecifier){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $4->typeSpecifier) , $4->tacResult);
                $4->tacResult = temp;
            }
            
            // Convert right operand to boolean if needed
            if($4->trueList.empty() && $4->falseList.empty()) {
                $4->trueList.push_back(codeGen.currentInstrIndex);
                codeGen.emit(TACOp::NE, "", $4->tacResult, "0", true);
                $4->falseList.push_back(codeGen.currentInstrIndex);
                codeGen.emit(TACOp::oth, "", "", "", true);
            }
            
            // Backpatch left's trueList to jump to right operand evaluation
            // Use $3->tacResult which is the M marker's saved index
            Backpatch::backpatch($1->trueList, $3->tacResult);
            
            // Result truelist is right's truelist, falselist is merge of both falselists
            $$->trueList = $4->trueList;
            $$->falseList = Backpatch::merge($1->falseList, $4->falseList);
            
            string temp = codeGen.newTemp();
            $$->tacResult = temp;
        } else {
            // Type compatibility check
        }
    }
	;

logical_or_expression
	: logical_and_expression { $$ = $1; }
	| logical_or_expression LOGICAL_OR_OPERATOR M logical_and_expression { 
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $4->pointerLevel;
        $$ = createNode(NODE_LOGICAL_OR_EXPRESSION, $2, $1, $4); 
        $$->isLValue = false;
        if (isTypeCompatible($1->typeSpecifier, $4->typeSpecifier, "||")) {
            $$->typeSpecifier = 3;
            if($1->typeSpecifier != $4->typeSpecifier){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $4->typeSpecifier) , $4->tacResult);
                $4->tacResult = temp;
            }
            
            // Convert right operand to boolean if needed (already converted by logical_and_expression rule)
            // Note: This should rarely be needed since right operand goes through logical_and_expression
            if($4->trueList.empty() && $4->falseList.empty()) {
                $4->trueList.push_back(codeGen.currentInstrIndex);
                codeGen.emit(TACOp::NE, "", $4->tacResult, "0", true);
                $4->falseList.push_back(codeGen.currentInstrIndex);
                codeGen.emit(TACOp::oth, "", "", "", true);
            }
            
            // Backpatch left's falseList to jump to right operand evaluation (for OR, we check right if left is false)
            // Use $3->tacResult which is the M marker's saved index
            Backpatch::backpatch($1->falseList, $3->tacResult);
            
            // Result falselist is right's falselist, truelist is merge of both truelists
            $$->falseList = $4->falseList;
            $$->trueList = Backpatch::merge($1->trueList, $4->trueList);
            
            string temp = codeGen.newTemp();
            $$->tacResult = temp;
        } else {
            // Type compatibility check
        }
    }
	;

M
  : {
      $$ = new TreeNode(OTHERS);
      // Store line number (1-indexed) for goto targets, not instruction index (0-indexed)
      $$->tacResult = to_string(codeGen.currentInstrIndex);
    }


conditional_expression
    : logical_or_expression { 
        $$ = $1; 
    }
    | logical_or_expression TERNARY_OPERATOR expression COLON conditional_expression { 
        $$ = createNode(NODE_CONDITIONAL_EXPRESSION, "?:", $1, $3, $5);
        $$->isLValue = false;
        
        if ($1->typeSpecifier < 0 || $1->typeSpecifier > 7) {
            cerr << "Ternary operator condition must be boolean, got type " 
                 << $1->typeSpecifier << " at line " << yylineno << endl;
        }
        if (isTypeCompatible($3->typeSpecifier, $5->typeSpecifier, "?:")!=-1) {
            $$->typeSpecifier = max($3->typeSpecifier, $5->typeSpecifier);
        } else {
            cerr << "Incompatible types in ternary operator: " 
                 << $3->typeSpecifier << " and " << $5->typeSpecifier 
                 << " at line " << yylineno << endl;
        }
        
        // Generate 3AC for ternary operator
        // Pattern: condition ? true_expr : false_expr
        
        // Backpatch the condition if it has control flow (from relational/logical ops)
        if (!$1->trueList.empty() || !$1->falseList.empty()) {
            // Condition is a boolean expression with control flow - convert to value
            Backpatch::backpatch($1->trueList, to_string(codeGen.currentInstrIndex));
            Backpatch::backpatch($1->falseList, to_string(codeGen.currentInstrIndex + 1));
            string condTemp = codeGen.newTemp();
            codeGen.emit(TACOp::ASSIGN, condTemp, "1", nullopt);
            codeGen.emit(TACOp::ASSIGN, condTemp, "0", nullopt);
            $1->tacResult = condTemp;
        }
        
        emitIndexIfNeeded($1);  // Emit INDEX if condition is array access
        emitIndexIfNeeded($3);  // Emit INDEX if true_expr is array access  
        emitIndexIfNeeded($5);  // Emit INDEX if false_expr is array access
        
        string labelTrue = codeGen.newLabel();
        string labelEnd = codeGen.newLabel();
        string temp = codeGen.newTemp();
        
        // Condition evaluation and jump to TRUE branch
        codeGen.emit(TACOp::NE, labelTrue, $1->tacResult, "0", true);
        
        // FALSE branch
        codeGen.emit(TACOp::ASSIGN, temp, $5->tacResult, nullopt);
        codeGen.emit(TACOp::GOTO, labelEnd, nullopt, nullopt, false);
        
        // TRUE branch label
        codeGen.emit(TACOp::LABEL, labelTrue);
        codeGen.emit(TACOp::ASSIGN, temp, $3->tacResult, nullopt);
        
        // END label
        codeGen.emit(TACOp::LABEL, labelEnd);
        
        $$->tacResult = temp;
    }
    ;

assignment_expression
    : conditional_expression { 
        $$ = $1; 
    }
    | unary_expression assignment_operator assignment_expression { 
        $$ = createNode(NODE_ASSIGNMENT_EXPRESSION, $2->value, $1, $3);
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        
        // Allow string literal to any pointer assignment (common in C for char*)
        bool isStringToPointerAssignment = ($1->pointerLevel > 0 && $3->typeSpecifier == 8 && $3->pointerLevel == 0);
        
        // Allow NULL (integer 0) assignment to pointers  
        bool isNullToPointerAssignment = (rhsPointerLevel > 0 && lhsPointerLevel == 0 && 
                                         $3->typeSpecifier == 3 &&
                                         ($3->tacResult == "0" || $3->valueToString() == "0"));
        
        if(rhsPointerLevel!=lhsPointerLevel && !($1->typeSpecifier == 3 && $3->typeCategory == 2) && !isStringToPointerAssignment && !isNullToPointerAssignment){
                cerr << "Incompatible types in compound assignment: " 
                     << "lhs(" << $1->typeSpecifier << ", ptr=" << $1->pointerLevel << ") and " 
                     << "rhs(" << $3->typeSpecifier << ", ptr=" << $3->pointerLevel << ")"
                     << " at line " << yylineno << endl;
        }
        else{
        if (!$1->isLValue) {
            cerr << "Left operand of assignment must be an L-value at line " 
                 << yylineno << endl;
        }
        
        if ($1->isConst) {
            cerr << "Left operand of assignment is constant at line " 
                 << yylineno << endl;
        }
        string op = $2->valueToString();
        if (op == "=") {
            // For struct/union types (typeSpecifier == 20), just check if types and pointer levels match
            bool isStructAssignment = ($1->typeSpecifier == 20 && $3->typeSpecifier == 20 && $1->pointerLevel == $3->pointerLevel);
            
            // Allow void* (type=0) to be assigned to any pointer type, or any pointer to void*
            bool isVoidPtrAssignment = ($1->pointerLevel > 0 && $3->pointerLevel > 0 && 
                                       ($1->typeSpecifier == 0 || $3->typeSpecifier == 0));
            
            if (isStructAssignment || isStringToPointerAssignment || isNullToPointerAssignment || isVoidPtrAssignment || isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, op)) {
                $$->typeSpecifier = $1->typeSpecifier;
                if($1->typeSpecifier != $3->typeSpecifier && !isStringToPointerAssignment && !isNullToPointerAssignment && !isVoidPtrAssignment){
                    string temp = codeGen.newTemp();
                    codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                    $3->tacResult = temp;
                }
            } else {
                cerr << "Incompatible types in assignment: " 
                     << "lhs(" << $1->typeSpecifier << ", ptr=" << $1->pointerLevel << ") and " 
                     << "rhs(" << $3->typeSpecifier << ", ptr=" << $3->pointerLevel << ")"
                     << " at line " << yylineno << endl;
            }
        } else {
            if ($1->typeSpecifier < 0 || $1->typeSpecifier > 6) { 
                cerr << "Compound assignment requires numeric type, got " 
                     << $1->typeSpecifier << " at line " << yylineno << endl;
            }
            if (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, op)) {
                $$->typeSpecifier = $1->typeSpecifier;
                if($1->typeSpecifier != $3->typeSpecifier){
                    string temp = codeGen.newTemp();
                    codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                    $3->tacResult = temp;
            }
            } else {
                cerr << "Incompatible types in compound assignment: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
            }
        }
        // if ($3->trueList || $3->falseList) {
        //     Backpatch::backpatch($3->trueList, to_string(codeGen.currentInstrIndex));
        //     Backpatch::backpatch($3->falseList, to_string(codeGen.currentInstrIndex + 1));
        //     handleAssignment($2->valueToString(), $1->tacResult, "1", true);
        //     handleAssignment($2->valueToString(), $1->tacResult, "0", true);
        // } else {
        //     handleAssignment($2->valueToString(), $1->tacResult, $3->tacResult);
        // }
        string temp = codeGen.newTemp();
        if($2->valueToString() == "="){
            // Emit INDEX for RHS if it's an array access (used as rvalue)
            emitIndexIfNeeded($3);
            
            // Check if LHS is an array/pointer index access - need to emit array store instead of simple assign
            if ($1->isArrayIndex && $1->children.size() == 2) {
                // This is arr[index] or arr[i][j] = value
                // Extract all indices and flatten for multi-dimensional arrays
                auto [arrayName, indices] = extractArrayIndices($1);
                
                // Look up array dimensions
                TreeNode* arrayNode = lookupSymbol(arrayName);
                vector<int> dimensions;
                if (arrayNode && arrayNode->typeCategory == 2) {
                    dimensions = arrayNode->dimensions;
                }
                
                // Handle struct member arrays (e.g., g.matrix)
                if (dimensions.empty() && arrayName.find('.') != string::npos) {
                    size_t dotPos = arrayName.find('.');
                    string structName = arrayName.substr(0, dotPos);
                    string memberName = arrayName.substr(dotPos + 1);
                    
                    TreeNode* structNode = lookupSymbol(structName);
                    if (structNode && structNode->symbolTable.size() > 0) {
                        for (const auto& [name, memberNode] : structNode->symbolTable) {
                            if (name == memberName && memberNode->typeCategory == 2) {
                                dimensions = memberNode->dimensions;
                                break;
                            }
                        }
                    }
                }
                
                // Flatten the indices if multi-dimensional
                string flatIndex;
                if (indices.size() > 1 && dimensions.size() > 1) {
                    flatIndex = flattenArrayIndex(arrayName, indices, dimensions);
                } else {
                    flatIndex = indices.empty() ? "0" : indices[0];
                }
                
                codeGen.emit(TACOp::ARRAY_STORE, arrayName, flatIndex, $3->tacResult);
            } else if ($1->type == NODE_UNARY_EXPRESSION && $1->children.size() >= 2 && 
                       $1->children[0]->valueToString() == "*") {
                // This is *ptr = value - need to emit indirect store
                // The dereferenced pointer's value is in children[1]->tacResult
                string ptrName = $1->children[1]->tacResult;
                codeGen.emit(TACOp::ASSIGN, "*" + ptrName, $3->tacResult, nullopt);
            } else if (!$1->basePointer.empty()) {
                // This is (*ptr).member = value - convert to ptr->member = value
                // Extract member name from tacResult (e.g., "t3.x" -> "x")
                string memberAccess = $1->tacResult;
                size_t dotPos = memberAccess.find('.');
                if (dotPos != string::npos) {
                    string memberName = memberAccess.substr(dotPos + 1);
                    // Emit: ptr->member = value
                    codeGen.emit(TACOp::ASSIGN, $1->basePointer + "->" + memberName, $3->tacResult, nullopt);
                } else {
                    // Fallback to regular assignment if something went wrong
                    codeGen.emit(TACOp::ASSIGN, $1->tacResult, $3->tacResult, nullopt);
                }
            } else {
                codeGen.emit(TACOp::ASSIGN, $1->tacResult, $3->tacResult, nullopt);
            }
        }else if($2->valueToString() == "+="){
            emitIndexIfNeeded($1);  // LHS used as rvalue
            emitIndexIfNeeded($3);  // RHS used as rvalue
            codeGen.emit(TACOp::ADD, $1->tacResult, $1->tacResult, $3->tacResult);
        }else if($2->valueToString() == "-="){
            emitIndexIfNeeded($1);
            emitIndexIfNeeded($3);
            codeGen.emit(TACOp::SUB, $1->tacResult, $1->tacResult, $3->tacResult);
        }else if($2->valueToString() == "*="){
            emitIndexIfNeeded($1);
            emitIndexIfNeeded($3);
            codeGen.emit(TACOp::MUL, $1->tacResult, $1->tacResult, $3->tacResult);
        }else if($2->valueToString() == "/="){
            emitIndexIfNeeded($1);
            emitIndexIfNeeded($3);
            codeGen.emit(TACOp::DIV, $1->tacResult, $1->tacResult, $3->tacResult);
        }else if($2->valueToString() == "%="){
            emitIndexIfNeeded($1);
            emitIndexIfNeeded($3);
            codeGen.emit(TACOp::MOD, $1->tacResult, $1->tacResult, $3->tacResult);
        }else if($2->valueToString() == "&="){
            emitIndexIfNeeded($1);
            emitIndexIfNeeded($3);
            codeGen.emit(TACOp::BIT_AND, $1->tacResult, $1->tacResult, $3->tacResult);
        }else if($2->valueToString() == "|="){
            emitIndexIfNeeded($1);
            emitIndexIfNeeded($3);
            codeGen.emit(TACOp::BIT_OR, $1->tacResult, $1->tacResult, $3->tacResult);
        }else if($2->valueToString() == "^="){
            emitIndexIfNeeded($1);
            emitIndexIfNeeded($3);
            codeGen.emit(TACOp::BIT_XOR, $1->tacResult, $1->tacResult, $3->tacResult);
        }else if($2->valueToString() == "<<="){
            emitIndexIfNeeded($1);
            emitIndexIfNeeded($3);
            codeGen.emit(TACOp::LSHFT,$1->tacResult,$1->tacResult,$3->tacResult);
        }else if($2->valueToString() == ">>="){
            emitIndexIfNeeded($1);
            emitIndexIfNeeded($3);
            codeGen.emit(TACOp::RSHFT,$1->tacResult,$1->tacResult,$3->tacResult);
        }
        // Result of assignment expression is the LHS value (after assignment)
        $$->tacResult = $1->tacResult;
        // CRITICAL: Clear nextList for assignments - they don't have control flow
        $$->nextList.clear();
        $$->trueList.clear();
        $$->falseList.clear();
    }
    }
    ;


assignment_operator
	: ASSIGNMENT_OPERATOR { $$ = createNode(NODE_ASSIGNMENT_OPERATOR, $1); }
	| MULTIPLY_ASSIGN_OPERATOR { $$ = createNode(NODE_ASSIGNMENT_OPERATOR, $1); }
	| DIVIDE_ASSIGN_OPERATOR { $$ = createNode(NODE_ASSIGNMENT_OPERATOR, $1); }
	| MODULO_ASSIGN_OPERATOR { $$ = createNode(NODE_ASSIGNMENT_OPERATOR, $1); }
	| PLUS_ASSIGN_OPERATOR { $$ = createNode(NODE_ASSIGNMENT_OPERATOR, $1); }
	| MINUS_ASSIGN_OPERATOR { $$ = createNode(NODE_ASSIGNMENT_OPERATOR, $1); }
	| LEFT_SHIFT_ASSIGN_OPERATOR { $$ = createNode(NODE_ASSIGNMENT_OPERATOR, $1); } 
	| RIGHT_SHIFT_ASSIGN_OPERATOR { $$ = createNode(NODE_ASSIGNMENT_OPERATOR, $1); }
	| BITWISE_AND_ASSIGN_OPERATOR { $$ = createNode(NODE_ASSIGNMENT_OPERATOR, $1); }
	| BITWISE_XOR_ASSIGN_OPERATOR { $$ = createNode(NODE_ASSIGNMENT_OPERATOR, $1); }
 	| BITWISE_OR_ASSIGN_OPERATOR { $$ = createNode(NODE_ASSIGNMENT_OPERATOR, $1); }
	;

expression
	: assignment_expression { $$ = $1; }
	| expression COMMA assignment_expression { $$ = createNode(NODE_EXPRESSION, monostate(), $1, $3); }
	;

constant_expression
	: conditional_expression { $$ = $1;$$->type = NODE_CONSTANT_EXPRESSION; }
	;

single_expression
    : expression{
        $$ = $1;
        // Always create trueList and falseList for control flow expressions
        // Check if they already exist (from boolean operations)
        if($$->trueList.empty() && $$->falseList.empty()) {
            // No existing lists - create conditional jump for true case
            $$->trueList.push_back(codeGen.currentInstrIndex);
            codeGen.emit(TACOp::NE, "", $1->tacResult, "0", true);
            // Create unconditional goto for false case
            $$->falseList.push_back(codeGen.currentInstrIndex);
            codeGen.emit(TACOp::oth, "", "", "", true);
        }
    }
    ;

declaration
    : declaration_specifiers SEMICOLON {
        $$ = $1;
    }
    | declaration_specifiers init_declarator_list SEMICOLON 
    {
        $$ = createNode(NODE_DECLARATION, monostate(), $1, $2);
        
        // Register typedef names immediately before processing
        registerTypedefNamesFromDeclaration($1, $2);
        
        bool isCustom = false;
        for(auto child: $1->children){
            if(child->type == NODE_STRUCT_OR_UNION_SPECIFIER){
                isCustom = true;
                break;
            }
        }
        if(isCustom){
            // Check if we have storage class or qualifiers (but allow typedef)
            bool hasNonTypedefStorage = false;
            bool isTypedef = false;
            for(auto child: $1->children){
                if(child->type == NODE_STORAGE_CLASS_SPECIFIER){
                    string storageVal = child->valueToString();
                    if(storageVal == "typedef"){
                        isTypedef = true;
                    } else {
                        hasNonTypedefStorage = true;
                    }
                } else if(child->type == NODE_TYPE_QUALIFIER){
                    hasNonTypedefStorage = true;
                }
            }
            
            if(hasNonTypedefStorage){
                cerr << "Error: Type Qualifier or Storage Class Specifier (other than typedef) is not allowed to be used with struct or union" << endl;
                break;
            }
            
            // If typedef, process normally through addDeclarators
            if(isTypedef){
                addDeclarators($1, $2);
            } else if($1->children.size() > 1){
                // Non-typedef with multiple children - shouldn't happen now
                cerr << "Error: Type Qualifier or Storage Class Specifier is not allowed to be used with struct or union" << endl;
                break;
            } else {
                // Regular struct declaration
                TreeNode* newDeclSpec = createNode(NODE_DECLARATION_SPECIFIERS, monostate(), $1->children[0]->children[1]);
                $1->children[0]->children[1]->type = NODE_TYPE_SPECIFIER;
                addDeclarators(newDeclSpec, $2);
                $1->children[0]->children[1]->type = NODE_IDENTIFIER;
                delete newDeclSpec;
            }
        }else{
            addDeclarators($1, $2);
        }
    };


declaration_specifiers
	: storage_class_specifier { $$ = createNode(NODE_DECLARATION_SPECIFIERS, monostate(), $1); }
	| storage_class_specifier declaration_specifiers { 
        $$ = createNode(NODE_DECLARATION_SPECIFIERS, monostate(), $1);
        for(auto child : $2->children){
            $$->addChild(child);
        }
    }
	| type_specifier { 
        $$ = createNode(NODE_DECLARATION_SPECIFIERS, monostate(), $1);
        $$->storageClass = $1->storageClass;
        $$->typeSpecifier = $1->typeSpecifier;
        $$->typeCategory = $1->typeCategory;
    }
	| type_specifier declaration_specifiers { 
        $$ = createNode(NODE_DECLARATION_SPECIFIERS, monostate(), $1);
        for(auto child : $2->children){
            $$->addChild(child);
        }
        $$->storageClass = $1->storageClass;
        $$->typeSpecifier = $1->typeSpecifier;
        $$->typeCategory = $1->typeCategory;
    }
	| type_qualifier { $$ = createNode(NODE_DECLARATION_SPECIFIERS, monostate(), $1); $$->storageClass = $1->storageClass;}
	| type_qualifier declaration_specifiers { 
        $$ = createNode(NODE_DECLARATION_SPECIFIERS, monostate(), $1);
        for(auto child : $2->children){
            $$->addChild(child);
        }
         
    }
	;    

init_declarator_list
    : init_declarator { 
        $$ = createNode(NODE_DECLARATOR_LIST, monostate(), $1); 
    }
    | init_declarator_list COMMA init_declarator { 
        $$ = $1;
        $$->children.push_back($3);
    }
    ;

init_declarator
    : declarator { 
        $$ = createNode(NODE_DECLARATOR, monostate(), $1, nullptr); 
    }
    | declarator ASSIGNMENT_OPERATOR initializer { 
        $$ = createNode(NODE_DECLARATOR, $2, $1, $3);   
        $$->storageClass = $3->storageClass;
    }
    ;

storage_class_specifier
    : KEYWORD_TYPEDEF   { $$ = $1; $$->type = NODE_STORAGE_CLASS_SPECIFIER; }
    | KEYWORD_EXTERN    { $$ = $1; $$->type = NODE_STORAGE_CLASS_SPECIFIER; }
    | KEYWORD_STATIC    { $$ = $1; $$->type = NODE_STORAGE_CLASS_SPECIFIER; }
    | KEYWORD_AUTO      { $$ = $1; $$->type = NODE_STORAGE_CLASS_SPECIFIER; }
    | KEYWORD_REGISTER  { $$ = $1; $$->type = NODE_STORAGE_CLASS_SPECIFIER; }
    ;


type_specifier
	: struct_type_specifier { $$ = $1; }
    | struct_or_union_specifier {
        $$ = $1;
        $$->type = NODE_TYPE_SPECIFIER;
        $$->typeSpecifier = 20;
        $$->typeCategory = 4;
    }
    | class_specifier { $$ = $1;}
	;

struct_type_specifier
	: KEYWORD_VOID { $$ = $1; $$->type = NODE_TYPE_SPECIFIER; $$->storageClass = 0;}
	| KEYWORD_CHAR { $$ = $1; $$->type = NODE_TYPE_SPECIFIER; $$->storageClass = 1;}
    | KEYWORD_SHORT { $$ = $1; $$->type = NODE_TYPE_SPECIFIER; $$->storageClass = 2;}
    | KEYWORD_INT { $$ = $1; $$->type = NODE_TYPE_SPECIFIER; $$->storageClass = 3;}
    | KEYWORD_LONG { $$ = $1; $$->type = NODE_TYPE_SPECIFIER; $$->storageClass = 4;}
    | KEYWORD_BOOL { $$ = $1; $$->type = NODE_TYPE_SPECIFIER; $$->storageClass = 7;}
    | KEYWORD_FLOAT { $$ = $1; $$->type = NODE_TYPE_SPECIFIER; $$->storageClass = 12;}
    | KEYWORD_DOUBLE { $$ = $1; $$->type = NODE_TYPE_SPECIFIER; $$->storageClass = 13;}
    | KEYWORD_SIGNED { $$ = $1; $$->type = NODE_TYPE_SPECIFIER; }
    | KEYWORD_UNSIGNED { $$ = $1; $$->type = NODE_TYPE_SPECIFIER; }
    | TYPE_NAME {
        $$ = $1; 
        $$->type = NODE_TYPE_SPECIFIER;
        // Type names (struct/union/typedef) - look up to get actual type
        string typeName = $1->valueToString();
        TreeNode* typeDef = lookupSymbol(typeName);
        if (typeDef && typeDef->typeSpecifier != -1) {
            $$->storageClass = typeDef->typeSpecifier;
            $$->typeSpecifier = typeDef->typeSpecifier;
            $$->typeCategory = typeDef->typeCategory;
            $$->pointerLevel = typeDef->pointerLevel;
        } else {
            // If lookup fails, assume it's a struct type (20)
            $$->storageClass = 20;
            $$->typeSpecifier = 20;
            $$->typeCategory = 4;
            $$->pointerLevel = 0;
        }
    }
    | TYPEDEF_NAME {
        $$ = $1; 
        $$->type = NODE_TYPE_SPECIFIER;
        // Typedef names - look up to get actual type
        string typedefName = $1->valueToString();
        TreeNode* typedefDef = lookupSymbol(typedefName);
        if (typedefDef && typedefDef->typeSpecifier != -1) {
            $$->storageClass = typedefDef->typeSpecifier;
            $$->typeSpecifier = typedefDef->typeSpecifier;
            $$->typeCategory = typedefDef->typeCategory;
            $$->pointerLevel = typedefDef->pointerLevel;
        } else {
            // If lookup fails, assume it's a struct type (20)
            $$->storageClass = 20;
            $$->typeSpecifier = 20;
            $$->typeCategory = 4;
            $$->pointerLevel = 0;
        }
    }
    | struct_or_union_specifier {
        $$ = $1;
        // Mark this as a type specifier for declaration processing
        $$->type = NODE_TYPE_SPECIFIER;
        $$->typeSpecifier = 20; // Custom type
        $$->typeCategory = 4;   // Class/struct/union
    }
    | enum_specifier {
        $$ = $1;
        $$->type = NODE_TYPE_SPECIFIER;
        $$->typeSpecifier = 3;  // Enums are treated as int
        $$->typeCategory = 0;
    }
	;

class_specifier
    : {if (insideClass) yyerror("Nested classes are not allowed.");} 
        KEYWORD_CLASS ID LBRACE 
        {
            classOrStructOrUnion.insert($3->valueToString());
            insideClass = true;
            enterScope();
        }
      member_declaration_list RBRACE 
        {
            $$ = createNode(NODE_CLASS_SPECIFIER, monostate(), $3, $6);
            exitScope();
            insideClass = false;
        }
    ;

member_declaration_list
    : member_declaration
        { $$ = createNode(NODE_MEMBER_DECLARATION_LIST, monostate(), $1); }
    | member_declaration_list member_declaration
        { $$ = $1; $$->children.push_back($2); }
    ;

member_declaration
    : access_specifier COLON
        { $$ = createNode(NODE_ACCESS_SPECIFIER, monostate(), $1); }
    | declaration
        { $$ = $1; }
    | constructor_function
        { $$ = $1; }
    | function_definition
        { $$ = $1; }
    | destructor_function
        { $$ = $1; }
    ;


access_specifier
    : KEYWORD_PUBLIC {accessSpecifier = 1; $$ = $1; }
    | KEYWORD_PRIVATE {accessSpecifier = 0; $$ = $1; }
    | KEYWORD_PROTECTED {accessSpecifier = 2;  $$ = $1; }
    ;

struct_or_union_specifier
    : struct_or_union ID {
            string varName = $2->valueToString();
            classOrStructOrUnion.insert(varName);
            auto checkDuplicate = [&](const string &name) {
            for (const auto &entry : currentTable->symbolTable)
            {
                if (entry.first == name)
                {
                    cerr << "Error: Duplicate declaration of '" << name << "'\n";
                    return true;
                }
            }
            return false;
            };
            if(!checkDuplicate(varName)){
                insertSymbol(varName, $2);
                $2->typeCategory = 4;
                $2->typeSpecifier = 20;
            }
            enterScope();
    } LBRACE struct_declaration_list RBRACE {
            $$ = createNode(NODE_STRUCT_OR_UNION_SPECIFIER,monostate(), $1, $2, $5);
            $2->symbolTable = currentTable->symbolTable;
            exitScope();
    }
    | struct_or_union {
            // Anonymous struct/union - generate a unique name
            static int anonCount = 0;
            string anonName = "__anon_" + string($1->valueToString()) + "_" + to_string(anonCount++);
            TreeNode* nameNode = createNode(NODE_IDENTIFIER, anonName);
            classOrStructOrUnion.insert(anonName);
            insertSymbol(anonName, nameNode);
            nameNode->typeCategory = 4;
            nameNode->typeSpecifier = 20;
            enterScope();
            $<node>$ = nameNode;  // Save name node for later
    } LBRACE struct_declaration_list RBRACE {
            TreeNode* nameNode = $<node>2;
            $$ = createNode(NODE_STRUCT_OR_UNION_SPECIFIER, monostate(), $1, nameNode, $4);
            nameNode->symbolTable = currentTable->symbolTable;
            exitScope();
    }
    | struct_or_union TYPE_NAME {
            // Using an already-defined struct/union type
            $$ = createNode(NODE_STRUCT_OR_UNION_SPECIFIER, monostate(), $1, $2);
    }
    | struct_or_union TYPEDEF_NAME {
            // Using a typedef'd struct/union type
            $$ = createNode(NODE_STRUCT_OR_UNION_SPECIFIER, monostate(), $1, $2);
    }
    ;

struct_or_union
    : KEYWORD_STRUCT { $$ = $1; }
    | KEYWORD_UNION { $$ = $1; }
    ;

enum_specifier
    : KEYWORD_ENUM ID LBRACE enumerator_list RBRACE {
            string enumName = $2->valueToString();
            // Note: Don't add to classOrStructOrUnion since enums are different
            $$ = createNode(NODE_ENUM_SPECIFIER, monostate(), $1, $2, $4);
            
            // Register enum constants in symbol table
            int enumValue = 0;
            for (auto &enumerator : $4->children) {
                string constName = enumerator->children[0]->valueToString();
                TreeNode* constNode = enumerator->children[0];
                
                // If enumerator has explicit value, use it
                if (enumerator->children.size() > 1) {
                    // Get the constant expression value
                    TreeNode* valueExpr = enumerator->children[1];
                    if (valueExpr->type == NODE_CONSTANT) {
                        enumValue = std::stoi(valueExpr->valueToString());
                    }
                }
                
                // Create constant node
                constNode->typeSpecifier = 3;  // int
                constNode->typeCategory = 0;   // value
                constNode->isConstant = true;
                constNode->constantValue = enumValue;
                
                // Insert into symbol table
                insertSymbol(constName, constNode);
                
                enumValue++;
            }
    }
    | KEYWORD_ENUM LBRACE enumerator_list RBRACE {
            // Anonymous enum
            $$ = createNode(NODE_ENUM_SPECIFIER, monostate(), $1, $3);
            
            // Register enum constants in symbol table
            int enumValue = 0;
            for (auto &enumerator : $3->children) {
                string constName = enumerator->children[0]->valueToString();
                TreeNode* constNode = enumerator->children[0];
                
                // If enumerator has explicit value, use it
                if (enumerator->children.size() > 1) {
                    TreeNode* valueExpr = enumerator->children[1];
                    if (valueExpr->type == NODE_CONSTANT) {
                        enumValue = std::stoi(valueExpr->valueToString());
                    }
                }
                
                constNode->typeSpecifier = 3;  // int
                constNode->typeCategory = 0;   // value
                constNode->isConstant = true;
                constNode->constantValue = enumValue;
                
                insertSymbol(constName, constNode);
                
                enumValue++;
            }
    }
    | KEYWORD_ENUM ID {
            // Reference to existing enum type (e.g., enum Color c;)
            $$ = createNode(NODE_ENUM_SPECIFIER, monostate(), $1, $2);
    }
    ;

enumerator_list
    : enumerator {
        $$ = createNode(NODE_ENUMERATOR_LIST, monostate(), $1);
    }
    | enumerator_list COMMA enumerator {
        $$ = $1;
        $$->children.push_back($3);
    }
    ;

enumerator
    : ID {
        $$ = createNode(NODE_ENUMERATOR, monostate(), $1);
    }
    | ID ASSIGNMENT_OPERATOR constant_expression {
        $$ = createNode(NODE_ENUMERATOR, monostate(), $1, $3);
    }
    ;

struct_declaration_list
    : struct_declaration { 
        $$ = createNode(NODE_STRUCT_DECLARATION_LIST, monostate(), $1); 
    }
    | struct_declaration_list struct_declaration { 
        $$ = $1;
        $$->children.push_back($2);
    }
    ;

struct_declaration
    : specifier_qualifier_list struct_declarator_list SEMICOLON {
        $$ = createNode(NODE_STRUCT_DECLARATION, monostate(), $1, $2);
        DeclaratorInfo declInfo = isValidVariableDeclaration($1->children, false);
        if (declInfo.isValid)
        {
    // For struct-typed members, look up the struct definition
    auto helper = $1;
    for (auto specChild : $1->children) {
        if (specChild->type == NODE_TYPE_SPECIFIER) {
            helper = specChild;
            break;
        }
    }
    TreeNode* typeInfo = helper;
    if (declInfo.typeCategory == 4) {
        // This is a struct/class/union type - look up its definition
        // For struct_or_union_specifier (which now has type NODE_TYPE_SPECIFIER), get the TYPE_NAME child
        string typeName;
        if (helper->children.size() >= 2) {
            // This is a struct_or_union_specifier with [struct_or_union, TYPE_NAME]
            typeName = helper->children[1]->valueToString();
        } else {
            typeName = helper->valueToString();
        }
        TreeNode* lookedUp = lookupSymbol(typeName);
        if (lookedUp != nullptr) {
            typeInfo = lookedUp;
        }
    }
    
    for (auto child : $2->children)
    {
        if (child->type != NODE_DECLARATOR) continue;

        TreeNode *firstChild = child->children[0];
        string varName;
        TreeNode *identifierNode = firstChild;
        auto setNodeAttributes = [&](TreeNode *node, int typeCategory, int pointerLevel = 0) {
            node->typeCategory = typeCategory;
            node->pointerLevel = pointerLevel;
            node->storageClass = declInfo.storageClass;
            node->typeSpecifier = declInfo.typeSpecifier;
            node->isConst = declInfo.isConst;
            node->isStatic = declInfo.isStatic;
            node->isVolatile = declInfo.isVolatile;
            node->isUnsigned = declInfo.isUnsigned;
            // Copy symbol table for struct-typed members
            if (typeInfo != nullptr) {
                node->symbolTable = typeInfo->symbolTable;
            }
        };
        auto checkDuplicate = [&](const string &name) {
            for (const auto &entry : currentTable->symbolTable)
            {
                if (entry.first == name)
                {
                    cerr << "Error: Duplicate declaration of '" << name << "'\n";
                    return true;
                }
            }
            return false;
        };

        if (firstChild->type == ARRAY)
        {
            vector<int> dimensions = findArrayDimensions(firstChild);
            while (identifierNode && identifierNode->type == ARRAY) {
                if (identifierNode->children.empty()) break;
                identifierNode = identifierNode->children[0];
            }
            varName = identifierNode->valueToString();
            if (checkDuplicate(varName)) continue;

            int size = child->children.size();
            if (size == 1 || size == 2) {
                bool validDims = all_of(dimensions.begin(), dimensions.end(), [](int d) { return d != -1; });
                if (!validDims) {
                    cerr << "Invalid declaration dimension cannot be empty\n";
                    continue;
                }
                if (size == 2 && !checkInitializerLevel(child->children[1], declInfo.typeSpecifier, dimensions, 0)) {
                    cerr << "Error\n";
                    continue;
                }
                setNodeAttributes(identifierNode, 2); 
                identifierNode->dimensions = dimensions;
                insertSymbol(varName, identifierNode);
            }
        }
        else if (firstChild->type == NODE_POINTER)
        {
            int pointerDepth = 0;
            while (identifierNode && identifierNode->type == NODE_POINTER) {
                pointerDepth++;
                if (identifierNode->children.empty()) break;
                identifierNode = identifierNode->children[0];
            }
            varName = identifierNode->valueToString();

            if (identifierNode->type == ARRAY) 
            {
                vector<int> dimensions = findArrayDimensions(identifierNode);
                varName = identifierNode->children[0]->valueToString();
                if (checkDuplicate(varName)) continue;

                int size = child->children.size();
                if (size == 1 || size == 2) {
                    bool validDims = all_of(dimensions.begin(), dimensions.end(), [](int d) { return d != -1; });
                    if (!validDims) {
                        cerr << "Invalid declaration dimension cannot be empty\n";
                        continue;
                    }
                    if (size == 2 && !checkInitializerLevel(child->children[1], declInfo.typeSpecifier, dimensions, pointerDepth)) {
                        cerr << "Error: Invalid initializer for array of pointers '" << varName << "'\n";
                        continue;
                    }
                    
                    setNodeAttributes(identifierNode, 2, pointerDepth);
                    identifierNode->dimensions = dimensions;
                    insertSymbol(varName, identifierNode);
                }
            }
            else 
            {
                if (checkDuplicate(varName)) continue;
                int size = child->children.size();
                if (size == 1 || (size == 2 )) {
                    setNodeAttributes(identifierNode, 1, pointerDepth);
                    insertSymbol(varName, identifierNode);
                }
                else {
                    cerr << "Error: Invalid pointer " << (size == 2 ? "initialization" : "declarator syntax") << " for '" << varName << "'\n";
                }
            }
        }
        else 
        {
            varName = firstChild->valueToString();
            if (checkDuplicate(varName)) continue;

            int size = child->children.size();
            if (size == 1) {
                if (declInfo.isConst) {
                    cerr << "Error: Const variable '" << varName << "' must be initialized\n";
                    continue;
                }
                setNodeAttributes(identifierNode, 0);
                insertSymbol(varName, identifierNode);
            }
            else if (size == 2 && (isTypeCompatible(declInfo.typeSpecifier, child->children[1]->typeSpecifier, "="))) {
                if(declInfo.typeSpecifier != child->children[1]->typeSpecifier){
                    string temp = codeGen.newTemp();
                    codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo(declInfo.typeSpecifier, child->children[1]->typeSpecifier) , child->children[1]->tacResult);
                    child->children[1]->tacResult = temp;
                }
                setNodeAttributes(identifierNode, 0);
                insertSymbol(varName, identifierNode);
            }
            else {
                cerr << "Error: " << (size == 2 ? "Type mismatch in initialization" : "Invalid declarator syntax") << " for '" << varName << "'\n";
            }
        }
}        }
    }
    ;

specifier_qualifier_list
	: struct_type_specifier specifier_qualifier_list {$$ = $2; $2->addChild($1);}
	| struct_type_specifier { $$ = createNode(NODE_SPECIFIER_QUALIFIER_LIST, monostate(), $1); }
	| type_qualifier specifier_qualifier_list { $$ = $2; $2->addChild($1); }
	| type_qualifier { $$ = createNode(NODE_SPECIFIER_QUALIFIER_LIST, monostate(), $1); }
	| storage_class_specifier specifier_qualifier_list { $$ = $2; $2->addChild($1); }
	| storage_class_specifier { $$ = createNode(NODE_SPECIFIER_QUALIFIER_LIST, monostate(), $1); }
	;

struct_declarator_list
    : declarator { 
        TreeNode* temp = createNode(NODE_DECLARATOR, monostate(), $1);
        $$ = createNode(NODE_STRUCT_DECLARATOR_LIST, monostate(), temp); 
    }
    | struct_declarator_list COMMA declarator { 
        TreeNode* temp = createNode(NODE_DECLARATOR, monostate(), $3);
        $$->children.push_back(temp);
    }
    ;
    
type_qualifier
	: KEYWORD_CONST { $$ = $1; $$->type = NODE_TYPE_QUALIFIER; }
	| KEYWORD_VOLATILE { $$ = $1; $$->type = NODE_TYPE_QUALIFIER; }
	;

declarator
    : pointer direct_declarator { 
        TreeNode* lastPointer = $1;
        while (!lastPointer->children.empty() && lastPointer->children[0]->type == NODE_POINTER) {
            lastPointer = lastPointer->children[0];
        }
        lastPointer->addChild($2);
        $$ = $1;
        $$->pointerLevel++;
    }
    | pointer reference direct_declarator {
        // Handle int* &ptr - reference to pointer
        TreeNode* lastPointer = $1;
        while (!lastPointer->children.empty() && lastPointer->children[0]->type == NODE_POINTER) {
            lastPointer = lastPointer->children[0];
        }
        TreeNode* refNode = createNode(NODE_REFERENCE, monostate(), $3);
        refNode->isReference = true;
        lastPointer->addChild(refNode);
        $$ = $1;
        $$->pointerLevel++;
    }
    | reference direct_declarator {
        $$ = createNode(NODE_REFERENCE, monostate(), $2);
        $$->isReference = true;
    }
    | direct_declarator { 
        $$ = $1; 
    }
    ;

direct_declarator
    : ID { 
        $$ = $1;
    }
    | LPAREN declarator RPAREN { 
        $$ = $2;
    }
    | direct_declarator LBRACKET INTEGER RBRACKET { 
        // Validate array size
        int arraySize = stoi($3->valueToString());
        if (arraySize <= 0) {
            cerr << "Error: Array size must be positive, got: " << arraySize << endl;
            YYABORT;
        }
        $$ = createNode(ARRAY, monostate(), $1, $3); 
    }
    | direct_declarator LBRACKET RBRACKET { 
        $$ = createNode(ARRAY, monostate(), $1, nullptr); 
    }
    | direct_declarator LPAREN parameter_type_list RPAREN { 
        $$ = createNode(NODE_DECLARATOR, monostate(), $1, $3); 
    }
    | direct_declarator LPAREN identifier_list RPAREN { 
        $$ = createNode(NODE_DECLARATOR, monostate(), $1, $3); 
    }
    | direct_declarator LPAREN RPAREN { 
        $$ = createNode(NODE_DECLARATOR, monostate(), $1, nullptr); 
    }
    ;

pointer
	: MULTIPLY_OPERATOR { $$ = createNode(NODE_POINTER, $1); }
	| MULTIPLY_OPERATOR type_qualifier_list { $$ = createNode(NODE_POINTER, $1, $2); }
	| MULTIPLY_OPERATOR pointer { $$ = createNode(NODE_POINTER, $1, $2); }
	| MULTIPLY_OPERATOR type_qualifier_list pointer { $$ = createNode(NODE_POINTER, $1, $2, $3); }
	;

reference
	: BITWISE_AND_OPERATOR { $$ = createNode(NODE_REFERENCE, $1); }
	| BITWISE_AND_OPERATOR type_qualifier_list { $$ = createNode(NODE_REFERENCE, $1, $2); }
	;

type_qualifier_list
    : type_qualifier { 
        $$ = createNode(NODE_TYPE_QUALIFIER_LIST, monostate(), $1); 
    }
    | type_qualifier_list type_qualifier { 
        $$ = $1;
        $$->children.push_back($2);
    }
    ;

parameter_type_list
    : parameter_list { 
        $$ = $1; 
    }
    | parameter_list COMMA ELLIPSIS_OPERATOR { 
        $$ = createNode(NODE_PARAMETER_TYPE_LIST, monostate(), $1);
        $$->children.push_back($3); 
    }
    ;


parameter_list
    : parameter_declaration { 
        $$ = createNode(NODE_PARAMETER_LIST, monostate(), $1); 
    }
    | parameter_list COMMA parameter_declaration { 
        $$ = $1;
        $$->children.push_back($3);
    }
    ;

parameter_declaration
	: declaration_specifiers declarator { $$ = createNode(NODE_PARAMETER_DECLARATION, monostate(), $1, $2); }
	| declaration_specifiers abstract_declarator { $$ = createNode(NODE_PARAMETER_DECLARATION, monostate(), $1, $2); }
	| declaration_specifiers { $$ = $1; }
	;

identifier_list
    : ID { 
        $$ = createNode(NODE_IDENTIFIER_LIST, monostate(), $1); 
    }
    | identifier_list COMMA ID { 
        $$ = $1;
        $$->children.push_back($3);
    }
    ;

type_name
	: specifier_qualifier_list { $$ = $1; }
	| specifier_qualifier_list abstract_declarator { $$ = createNode(NODE_TYPE_NAME, monostate(), $1, $2); }
	;

abstract_declarator
	: pointer {  $$ = $1;}
	| direct_abstract_declarator { $$ = createNode(NODE_ABSTRACT_DECLARATOR, monostate(), $1); }
	| pointer direct_abstract_declarator { $$ = createNode(NODE_ABSTRACT_DECLARATOR, monostate(), $1, $2); }
	;

direct_abstract_declarator
	: LPAREN abstract_declarator RPAREN {  $$ = $2; }
	| LBRACKET RBRACKET { $$ = createNode(NODE_DIRECT_ABSTRACT_DECLARATOR, monostate()); }
	| LBRACKET constant_expression RBRACKET {  $$ = $2; }
	| direct_abstract_declarator LBRACKET RBRACKET  {  $$ = $1; } 
	| direct_abstract_declarator LBRACKET constant_expression RBRACKET { $$ = createNode(NODE_DIRECT_ABSTRACT_DECLARATOR, monostate(), $1, $3); }
	| LPAREN RPAREN { $$ = createNode(NODE_DIRECT_ABSTRACT_DECLARATOR, monostate()); }
	| LPAREN parameter_type_list RPAREN {  $$ = $2; }
	| direct_abstract_declarator LPAREN RPAREN {  $$ = $1; }
	| direct_abstract_declarator LPAREN parameter_type_list RPAREN { $$ = createNode(NODE_DIRECT_ABSTRACT_DECLARATOR, monostate(), $1, $3); }
	;

initializer
	: assignment_expression { $$ = $1; }
	| LBRACE initializer_list RBRACE { $$ = $2;}
	| LBRACE initializer_list COMMA RBRACE { $$ = $2; }
	;

initializer_list
    : initializer { 
        $$ = createNode(NODE_INITIALIZER_LIST, monostate(), $1); 
    }
    | initializer_list COMMA initializer { 
        $$ = $1;
        $$->children.push_back($3);
    }
    ;

statement
	: labeled_statement { $$ = $1; }
	| compound_statement { $$ = $1;}
	| expression_statement { $$ = $1; }
	| selection_statement { 
        $$ = $1;
        // Don't backpatch here - let the parent block_item_list do it
        }
	| iteration_statement { 
        $$ = $1;
        // Don't backpatch here - let the parent block_item_list do it
        }
	| jump_statement { $$ = $1; }
    | io_statement{$$ = $1;}
    | scope_resolution_statement { $$ = $1; }
    ;

scope_resolution_statement
    : ID SCOPE_RESOLUTION_OPERATOR ID SEMICOLON { $$ = createNode(NODE_SCOPE_RESOLUTION_STATEMENT, monostate(), $1, $3); }
    | ID SCOPE_RESOLUTION_OPERATOR ID ASSIGNMENT_OPERATOR expression SEMICOLON { 
        $$ = createNode(NODE_SCOPE_RESOLUTION_STATEMENT, $4, $1, $3, $5); 
        string target = $1->valueToString() + "::" + $3->valueToString();
        codeGen.emit(TACOp::ASSIGN, target, $5->tacResult, nullopt);}
	;

io_statement
    : KEYWORD_PRINTF LPAREN STRING RPAREN SEMICOLON 
        {
            $3->tacResult = $3->valueToString();
            $$ = createNode(NODE_IO_STATEMENT, monostate(), $1, $3);  
            if (!checkFormatSpecifiers($3->valueToString(), {})) {
                cerr << "Error: Format string in printf has specifiers but no arguments provided" << endl;
                YYABORT;
            }else{
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::ASSIGN, "param", $3->tacResult);
                codeGen.emit(TACOp::CALL, temp, "printf", "1");   
            }
        }
    | KEYWORD_PRINTF LPAREN STRING COMMA argument_expression_list RPAREN SEMICOLON 
        {
            $3->tacResult = $3->valueToString();     
            $$ = createNode(NODE_IO_STATEMENT, monostate(), $1, $3, $5);          
            vector<int> types = typeExtract($5);
            if (!checkFormatSpecifiers($3->valueToString(), types)) {
                cerr << "Warning: Type mismatch between format specifiers and arguments in printf" << endl;
                // Continue compilation despite warning
            }
            codeGen.emit(TACOp::ASSIGN, "param", $3->tacResult);
            for (auto* arg : $5->children) {
                emitIndexIfNeeded(arg);  // Emit INDEX if argument is array access
                codeGen.emit(TACOp::ASSIGN, "param", arg->tacResult);
            }
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::CALL, temp, "printf", to_string($5->children.size() + 1));
            $$->tacResult = temp;   
        }
    | KEYWORD_SCANF LPAREN STRING COMMA argument_expression_list RPAREN SEMICOLON 
        {
            $3->tacResult = $3->valueToString();
            $$ = createNode(NODE_IO_STATEMENT, monostate(), $1, $3, $5); 
            
            // For scanf, we check argument count and pointer requirements
            if (!checkScanfArguments($3->valueToString(), $5)) {
                YYABORT;
            }
            codeGen.emit(TACOp::ASSIGN, "param", $3->tacResult);
            for (auto* arg : $5->children) {
                emitIndexIfNeeded(arg);  // Emit INDEX if argument is array access
                codeGen.emit(TACOp::ASSIGN, "param", arg->tacResult);
            }
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::CALL, temp, "scanf", to_string($5->children.size() + 1));
            $$->tacResult = temp;
        }
    ;

labeled_statement
	: ID { 
        codeGen.emit(TACOp::LABEL, $1->valueToString());
        if(Label_defn.find($1->valueToString())!=Label_defn.end()){
            cerr << "Re definition of lables is not allowed\n";
        }else{
            // currentInstrIndex has already been incremented by emit(), so it points to the next instruction
            Label_defn[$1->valueToString()] = to_string(codeGen.currentInstrIndex);
            
            // Backpatch any forward gotos to this label
            string labelName = $1->valueToString();
            if(forwardGotoRefs.count(labelName) > 0) {
                for(int instrIdx : forwardGotoRefs[labelName]) {
                    // Update the goto instruction with the actual label position
                    codeGen.tacCode[instrIdx].result = to_string(codeGen.currentInstrIndex);
                }
                // Clear the forward references for this label
                forwardGotoRefs.erase(labelName);
            }
        }
    } COLON statement { 
        $$ = createNode(NODE_LABELED_STATEMENT, monostate(), $1, $4);
        $1->typeCategory = 7;
        $1->typeSpecifier = 9;
        insertSymbol($1->valueToString(), $1);
        // Propagate control flow lists from statement
        $$->nextList = $4->nextList;
        $$->breakList = $4->breakList;
        $$->continueList = $4->continueList;
        }
    | KEYWORD_CASE constant_expression COLON {
        if(inSwitch.size() == 0) {
            cerr << "Error: Case label must be inside switch statement" << endl;
            YYABORT;
        }
        
        // Extract the case value - use tacResult which contains the computed constant
        // The parser already evaluates constant expressions and stores them in temporaries
        // For case labels, we need the actual constant value, not a temporary
        string caseValue;
        
        // Helper function to recursively evaluate constant expression
        function<string(TreeNode*)> evalConst = [&](TreeNode* node) -> string {
            if (!node) return "(null)";
            

            // First check if tacResult contains a constant value (not a temporary)
            if (!node->tacResult.empty() && node->tacResult[0] != 't' && node->tacResult[0] != '&' && node->tacResult[0] != '*') {
                // tacResult is not a temporary variable, might be the constant value
                string tac = node->tacResult;
                // Check if it's a number (positive or negative)
                if (tac[0] == '-' || isdigit(tac[0])) {
                    return tac;
                }
            }
            
            string val = node->valueToString();
            
            // Check if this is a unary expression node (NODE_UNARY_EXPRESSION = 60)
            if (node->type == 60 && node->children.size() >= 2) {
                // Unary expression has operator as first child, operand as second
                TreeNode* op = node->children[0];
                TreeNode* operand = node->children[1];
                string opVal = op->valueToString();
                
                if (opVal == "-") {
                    // Unary minus: recursively evaluate operand and negate
                    string operandVal = evalConst(operand);
                    if (operandVal != "(null)" && !operandVal.empty() && operandVal != "(unknown)") {
                        // Handle already negative numbers
                        if (operandVal[0] == '-') {
                            return operandVal.substr(1);  // Double negative = positive
                        }
                        return "-" + operandVal;
                    }
                } else if (opVal == "+") {
                    // Unary plus: just return operand value
                    return evalConst(operand);
                }
            }
            // If this node is a constant expression with children, check for operator pattern
            // Negative numbers are represented as: NODE_CONSTANT_EXPRESSION -> [operator_node, number_node]
            if (node->children.size() == 2) {
                TreeNode* first = node->children[0];
                TreeNode* second = node->children[1];
                
                // Check if first child is an operator (-, +, etc.)
                if (first->valueToString() == "-") {
                    string operand = evalConst(second);
                    if (operand != "(null)" && operand != "(unknown)") {
                        return "-" + operand;
                    }
                } else if (first->valueToString() == "+") {
                    return evalConst(second);
                }
            }
            
            // Check if this is an additive expression node with operator
            else if (node->type == 57 && val == "-") {
                // This is just the operator node, skip it (parent will handle)
                return "(null)";
            }
            // If we have a numeric value (not an operator symbol), return it
            else if (val != "(null)" && val != "(unknown)" && val != "-" && val != "+" && val != "*" && val != "/") {
                // Found a concrete numeric/identifier value
                return val;
            }
            
            // Traverse all children to find the value
            for (auto* child : node->children) {
                string childVal = evalConst(child);
                if (childVal != "(null)" && childVal != "(unknown)") {
                    return childVal;
                }
            }
            
            return "(null)";
        };
        
        caseValue = evalConst($2);
        
        // Check if the value is a variable by looking it up in symbol table
        // If it's found as a variable (not a constant like enum), it's not allowed
        TreeNode* symbol = lookupSymbol(caseValue, true);
        if(symbol != nullptr && !symbol->isConstant) {
            cerr << "Error: Case label must be a constant expression, not a variable: " << caseValue << endl;
            YYABORT;
        }
        
        // If it's an enum constant, use its integer value
        if(symbol != nullptr && symbol->isConstant) {
            caseValue = to_string(symbol->constantValue);
        }
        
        // Check for duplicate case values
        if(inSwitch.size() > 0) {
            for(const auto& existingCase : switchCases.top()) {
                if(existingCase.first == caseValue) {
                    cerr << "Error: Duplicate case value: " << caseValue << endl;
                    YYABORT;
                }
            }
        }
        
        // Generate label for this case
        string caseLabel = codeGen.newLabel();
        
        // Add this case to the list
        if(inSwitch.size() > 0) {
            switchCases.top().push_back(make_pair(caseValue, caseLabel));
        }
        
        codeGen.emit(TACOp::LABEL, caseLabel, nullopt, nullopt);
        $<node>$ = createNode(NODE_LABELED_STATEMENT, monostate());
        $<node>$->tacResult = caseLabel;
    } statement {
        $$ = createNode(NODE_LABELED_STATEMENT, monostate(), $2, $5);
        $$->nextList = $5->nextList;
        $$->breakList = $5->breakList;
        $$->continueList = $5->continueList;  // FIX: Propagate continueList for continue inside switch
    }
    | KEYWORD_DEFAULT COLON {
        if(inSwitch.size()==0){
            cerr << "Error: Default case should be used inside switch statement" << endl;
            YYABORT;
        }
        else if(inSwitch.top()){
            cerr << "Error: Multiple default labels in switch statement" << endl;
            YYABORT;
        }
        else{
            inSwitch.pop();
            inSwitch.push(true);
        }
        // Generate label for default case
        string defaultLabel = codeGen.newLabel();
        
        // Store default label
        if(inSwitch.size() > 0) {
            switchDefaultLabel.top() = defaultLabel;
        }
        
        codeGen.emit(TACOp::LABEL, defaultLabel, nullopt, nullopt);
        $<node>$ = createNode(NODE_LABELED_STATEMENT, monostate());
        $<node>$->tacResult = defaultLabel;
    } statement {
        $$ = createNode(NODE_LABELED_STATEMENT, monostate(), $4);
        $$->nextList = $4->nextList;
        $$->breakList = $4->breakList;
        $$->continueList = $4->continueList;  // FIX: Propagate continueList for continue inside switch
        
        // FIX: ALWAYS add implicit break after default case to prevent fallthrough into dispatch table
        // The dispatch code is generated after all cases, so falling through would create an infinite loop
        codeGen.emit(TACOp::GOTO, "DEFAULT_END_PLACEHOLDER");
        $$->breakList.push_back(codeGen.currentInstrIndex - 1);
    }
	;


compound_statement
    : LBRACE { enterScope(); } RBRACE {
        $$ = createNode(NODE_COMPOUND_STATEMENT, monostate());
        // Empty block needs a nextList for proper control flow
        $$->nextList = Backpatch::makeList(codeGen.currentInstrIndex);
        codeGen.emit(TACOp::GOTO, "NEXT");  // Will be backpatched to next statement
        exitScope();
    }
    | LBRACE { enterScope(); } block_item_list RBRACE {$$ = $3; exitScope(); }
    ;

block_item_list
    : block_item { 
        $$ = createNode(NODE_BLOCK_ITEM_LIST, monostate(), $1);
        $$->continueList = $1->continueList;
        $$->breakList = $1->breakList;
        $$->nextList = $1->nextList;  // Propagate nextList
    }
    | block_item_list M block_item { 
        $$ = $1; 
        $$->children.push_back($3);
        Backpatch::backpatch($1->nextList, $2->tacResult);
        $$->continueList = Backpatch::merge($1->continueList, $3->continueList);
        $$->breakList = Backpatch::merge($1->breakList, $3->breakList);
        $$->nextList = $3->nextList;
        }
    ;

block_item
    : declaration {
        $$ = $1;
        // Declarations don't have control flow
        if ($$) {
            $$->nextList.clear();
            $$->trueList.clear();
            $$->falseList.clear();
        }
    }
    | statement {
        $$ = $1;
    }
    ;

expression_statement
	: SEMICOLON { 
        $$ = createNode(NODE_EXPRESSION_STATEMENT, monostate()); 
        // Ensure nextList is empty for empty statements
        $$->nextList.clear();
    }
	| expression SEMICOLON { 
        $$ = $1; 
        // CRITICAL: Expression statements don't have control flow - clear all lists
        $$->nextList.clear();
        $$->trueList.clear();
        $$->falseList.clear();
    }
	;


selection_statement
    :KEYWORD_IF LPAREN single_expression RPAREN M statement N %prec NO_ELSE  {
        $$ = createNode(NODE_SELECTION_STATEMENT, monostate());
        Backpatch::backpatch($3->trueList, $5->tacResult); 
        BackpatchList merged = Backpatch::merge($3->falseList, $7->nextList); 
        $$->nextList = Backpatch::merge(merged, $6->nextList);
        $$->breakList = $6->breakList;
        $$->continueList = $6->continueList;
    }
    | KEYWORD_IF LPAREN single_expression RPAREN M statement N KEYWORD_ELSE M statement  {
        $$ = createNode(NODE_SELECTION_STATEMENT, monostate());
        Backpatch::backpatch($3->trueList, $5->tacResult); 
        Backpatch::backpatch($3->falseList, $9->tacResult);
        BackpatchList merged = Backpatch::merge($6->nextList, $7->nextList);
        $$->nextList = Backpatch::merge(merged, $10->nextList);
        $$->breakList = Backpatch::merge($6->breakList, $10->breakList);  // Merge breaks from both branches
        $$->continueList = Backpatch::merge($6->continueList, $10->continueList);  // Merge continues from both branches
    }
    | KEYWORD_SWITCH LPAREN expression RPAREN {
        inSwitch.push(false);
        switchExpr.push($3->tacResult); // Store the switch expression  
        switchCases.push(vector<pair<string, string>>()); // Initialize empty case list
        switchDefaultLabel.push(""); // Initialize empty default label
        
        // Generate a jump to skip over the comparisons we'll add later
        $<node>$ = createNode(OTHERS);
        $<node>$->tacResult = to_string(codeGen.currentInstrIndex); // Save index for the skip-jump
        codeGen.emit(TACOp::GOTO, "SWITCH_BODY_PLACEHOLDER");  // Will be backpatched to body start
    } compound_statement {
        $$ = createNode(NODE_SELECTION_STATEMENT, monostate(), $3, $6);
        
        // Get the saved information
        int skipJumpIdx = stoi($<node>5->tacResult);
        string expr = switchExpr.top();
        vector<pair<string, string>> cases = switchCases.top();
        string defaultLabel = switchDefaultLabel.top();
        
        // DON'T emit end label yet - we need to emit it AFTER the dispatch table
        
        // Propagate continueList from the body (for continue inside switch inside loop)
        $$->continueList = $6->continueList;
        
        // Now generate comparison code at current position (after the body)
        // and backpatch the skip-jump to jump here
        string comparisonLabel = codeGen.newLabel();
        codeGen.emit(TACOp::LABEL, comparisonLabel, nullopt, nullopt);
        
        // Backpatch the initial skip-jump to jump to comparison code
        codeGen.tacCode[skipJumpIdx].result = comparisonLabel;
        
        // Generate comparisons for all cases
        for(const auto& caseInfo : cases) {
            string caseValue = caseInfo.first;
            string caseLabel = caseInfo.second;
            
            // Generate: temp = (expr == caseValue); if temp goto caseLabel
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::EQ, temp, expr, caseValue);
            codeGen.emit(TACOp::IF_EQ, caseLabel, temp, "1");
        }
        
        // After all comparisons, jump to default or end
        // Generate end label AFTER dispatch table
        string endLabel = codeGen.newLabel();
        
        if(!defaultLabel.empty()) {
            codeGen.emit(TACOp::GOTO, defaultLabel);
        } else {
            codeGen.emit(TACOp::GOTO, endLabel);
        }
        
        // NOW emit the end label - break statements will jump here (after dispatch)
        codeGen.emit(TACOp::LABEL, endLabel, nullopt, nullopt);
        
        // Backpatch all break statements to the end of switch (after dispatch table)
        Backpatch::backpatch($6->breakList, endLabel);
        
        // Clean up stacks
        inSwitch.pop();
        switchExpr.pop();
        switchCases.pop();
        switchDefaultLabel.pop();
    }
    
iteration_statement
    : M KEYWORD_WHILE LPAREN single_expression RPAREN {enterScope(); inLoop++;} M statement  {
        $$ = createNode(NODE_ITERATION_STATEMENT, monostate());
        Backpatch::backpatch($4->trueList, $7->tacResult);
        Backpatch::backpatch($8->nextList, $1->tacResult);
        codeGen.emit(TACOp::oth, $1->tacResult, nullopt, nullopt, true);
        Backpatch::backpatch($8->continueList, $1->tacResult);
        $$->nextList = Backpatch::merge($8->breakList, $4->falseList);
        inLoop--;
        exitScope();
    }
    | M KEYWORD_UNTIL LPAREN single_expression RPAREN {enterScope(); inLoop++;} M statement  {
        $$ = createNode(NODE_ITERATION_STATEMENT, monostate());
        // until is like while(!(condition)), so swap true and false lists
        Backpatch::backpatch($4->falseList, $7->tacResult);
        Backpatch::backpatch($8->nextList, $1->tacResult);
        codeGen.emit(TACOp::oth, $1->tacResult, nullopt, nullopt, true);
        Backpatch::backpatch($8->continueList, $1->tacResult);
        $$->nextList = Backpatch::merge($8->breakList, $4->trueList);
        inLoop--;
        exitScope();
    }
    | KEYWORD_DO M {enterScope(); inLoop++;} statement M  {inLoop--; exitScope();} KEYWORD_WHILE LPAREN single_expression RPAREN{
        $$ = createNode(NODE_ITERATION_STATEMENT, monostate());
        Backpatch::backpatch($9->trueList, $2->tacResult);
        Backpatch::backpatch($4->continueList, $5->tacResult);
        $$->nextList = Backpatch::merge($4->breakList, $9->falseList);
    }
    | KEYWORD_FOR LPAREN {enterScope();} for_init M for_cond M for_inc M RPAREN {
            // $5 = M after init (marks start of condition)
            // $6 = for_cond (condition with trueList/falseList)
            // $7 = M after condition (marks start of increment)
            // $8 = for_inc (increment expression)
            // $9 = M after increment (marks where to jump after body)
            
            // After increment, emit goto back to condition
            codeGen.emit(TACOp::GOTO, $5->tacResult);
            
            // Save info for body processing
            $<node>$ = createNode(OTHERS);
            $<node>$->tacResult = to_string(codeGen.currentInstrIndex);  // Body start (line number)
            $<node>$->trueList = $6->trueList;   // Condition's trueList
            $<node>$->falseList = $6->falseList; // Condition's falseList  
            $<node>$->incStart = $7->tacResult;  // Increment start for continue (already +1 from M)
            
            inLoop++;
        } statement { 
            // $<node>11 = saved info (body start, condition lists, inc start)
            // $12 = statement (loop body)
            
            $$ = createNode(NODE_ITERATION_STATEMENT, monostate());
            
            // Emit goto from end of body to increment
            codeGen.emit(TACOp::GOTO, $<node>11->incStart);
            
            // Backpatch condition's trueList to body start
            Backpatch::backpatch($<node>11->trueList, $<node>11->tacResult);
            
            // Backpatch continue statements to increment start
            Backpatch::backpatch($12->continueList, $<node>11->incStart);
            
            // Backpatch body's nextList (e.g., inner loop exits) to increment
            // This ensures that when an inner loop finishes, we go to next iteration
            Backpatch::backpatch($12->nextList, $<node>11->incStart);
            
            // Merge break and condition falseList for exit
            $$->nextList = Backpatch::merge($12->breakList, $<node>11->falseList);
            
            exitScope();
            inLoop--; 
        }
    ;

for_init
    : expression_statement {$$ = $1;}
    | declaration {$$ = $1;}
    ;
for_cond
    : expression SEMICOLON {
        $$ = $1;
        // Unlike regular expression_statement, preserve trueList/falseList for loop condition
        // Convert to boolean if needed (like single_expression does)
        if($$->trueList.empty() && $$->falseList.empty()) {
            $$->trueList.push_back(codeGen.currentInstrIndex);
            codeGen.emit(TACOp::NE, "", $1->tacResult, "0", true);
            $$->falseList.push_back(codeGen.currentInstrIndex);
            codeGen.emit(TACOp::oth, "", "", "", true);
        }
    }
    | SEMICOLON {
        // Empty for condition means "true" - infinite loop
        $$ = createNode(NODE_EXPRESSION_STATEMENT, monostate());
        // Create a true condition (always jump to body, never exit)
        // Emit: goto (unconditional to body)
        $$->trueList.push_back(codeGen.currentInstrIndex);
        codeGen.emit(TACOp::oth, "", "", "", true);
        // falseList is empty - no way to exit
    }
    ;

for_inc
    : /* empty */ { $$ = createNode(NODE_EXPRESSION_STATEMENT, monostate()); }
    | expression {$$ = $1;}
    | expression_statement {$$ =$1;} 
    ;

jump_statement
	: KEYWORD_GOTO ID SEMICOLON { 
        $$ = createNode(NODE_JUMP_STATEMENT, monostate(), $1, $2);
        string labelName = $2->valueToString();
        
        if(Label_defn.count(labelName) > 0){
            // Backward goto - label already defined
            codeGen.emit(TACOp::GOTO, Label_defn[labelName]);
        }else{
            // Forward goto - store for backpatching later
            int gotoInstrIdx = codeGen.currentInstrIndex;
            codeGen.emit(TACOp::GOTO, "FORWARD_REF"); // Placeholder
            forwardGotoRefs[labelName].push_back(gotoInstrIdx);
        }
    }
	| KEYWORD_CONTINUE SEMICOLON {
        if(inLoop <= 0){
            cerr << "Error: Continue statement must be inside a loop" << endl;
            YYABORT;
        }
        // Note: Continue is allowed in a loop inside a switch
        $$ = $1;
        int gotoIdx = codeGen.emit(TACOp::GOTO, "");  // Will be backpatched later
        $$->continueList = Backpatch::makeList(gotoIdx);
    }
	| KEYWORD_BREAK SEMICOLON { 
        if(inLoop <= 0 && inSwitch.size() == 0){
            cerr << "Error: Break statement must be inside a loop or switch statement" << endl;
            YYABORT;
        }
        $$ = $1;
        int gotoIdx = codeGen.emit(TACOp::GOTO, "");  // Will be backpatched later
        $$->breakList = Backpatch::makeList(gotoIdx);
     }
	| KEYWORD_RETURN SEMICOLON { 
        if(!inFunc) {
            cerr << "Error: Return statement must be inside a function" << endl;
            YYABORT;
        }
        $$ = $1;
        if(expectedReturnType != 0){
            cerr << "Error: Expected return expression in non-void function.\n";
            YYABORT;
        }
        codeGen.emit(TACOp::RETURN, ""); 
        }
	| KEYWORD_RETURN expression SEMICOLON { 
        if(!inFunc) {
            cerr << "Error: Return statement must be inside a function" << endl;
            YYABORT;
        }
        $$ = createNode(NODE_JUMP_STATEMENT, monostate(), $1, $2);
        if(expectedReturnType == 0){
            cerr << "Error: Return statement with a value in void function.\n";
        }
        if(!isTypeCompatible(expectedReturnType, $2->typeSpecifier, "=")){
            cerr << "Error: Type mismatch in return statement.\n";
        }
        else{
            if(expectedReturnType != $2->typeSpecifier){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo(expectedReturnType, $2->typeSpecifier) , $2->tacResult);
                $2->tacResult = temp;
            }else{
                codeGen.emit(TACOp::RETURN, $2->tacResult);
            }
        } 
    }
	;

translation_unit
    : external_declaration {
        $$ = $1;
    }
    | translation_unit external_declaration {
        $$ = createNode(NODE_TRANSLATION_UNIT, monostate(), $1, $2);
    }
    ;


external_declaration
	: function_definition { $$ = $1;}
	| declaration { $$ = $1;}
    | scope_resolution_statements {$$=$1;}
    ;

scope_resolution_statements
    : ID SCOPE_RESOLUTION_OPERATOR ID SEMICOLON
    | ID SCOPE_RESOLUTION_OPERATOR ID assignment_operator expression SEMICOLON
    | ID SCOPE_RESOLUTION_OPERATOR ID LPAREN RPAREN SEMICOLON
    | ID SCOPE_RESOLUTION_OPERATOR ID LPAREN argument_expression_list RPAREN SEMICOLON
    ;

constructor_function
    : ID LPAREN {enterScope();} parameter_list RPAREN compound_statement {
        $$ = createNode(NODE_CONSTRUCTOR_FUNCTION, monostate(), $1, $4, $6);
        exitScope();
    }
    | ID LPAREN RPAREN {enterScope();} compound_statement {
        $$ = createNode(NODE_CONSTRUCTOR_FUNCTION, monostate(), $1, $5); exitScope();
    }
    ;

function_definition
    : declaration_specifiers declarator {
        DeclaratorInfo declInfo = isValidVariableDeclaration($1->children, true);
        if (declInfo.isValid) {
            // Check if return type is a reference
            TreeNode* actualDeclarator = $2;
            bool isRefReturn = false;
            if ($2->type == NODE_REFERENCE) {
                isRefReturn = true;
                if (!$2->children.empty()) {
                    actualDeclarator = $2->children[0];
                }
            }
            
            // Handle pointer declarators (e.g., int* funcName())
            // Unwrap NODE_POINTER to get to the direct_declarator
            // AND count the pointer levels for the return type
            int returnPointerLevel = 0;
            while (actualDeclarator->type == NODE_POINTER && !actualDeclarator->children.empty()) {
                returnPointerLevel++;
                actualDeclarator = actualDeclarator->children[0];
            }
            
            string funcName = actualDeclarator->children[0]->valueToString();
            codeGen.emit(TACOp::LABEL, funcName, nullopt, nullopt);
            
            // Check if function was forward declared
            TreeNode* existingFunc = lookupSymbol(funcName, true); // Don't print error
            TreeNode* funcNode;
            if (existingFunc && existingFunc->typeCategory == 3) {
                // Use the existing forward declaration node
                funcNode = existingFunc;
            } else {
                // No forward declaration, insert new symbol
                funcNode = actualDeclarator->children[0];
                insertSymbol(funcName, funcNode);
            }
            
            enterScope();
            
            // Track parameter names for this function so MIPS generator can handle them
            vector<string> paramNames;
            funcNode->storageClass = declInfo.storageClass;
            funcNode->typeSpecifier = declInfo.typeSpecifier;
            funcNode->pointerLevel = returnPointerLevel; // Set return type pointer level
            funcNode->isReference = isRefReturn;
            expectedReturnType = declInfo.typeSpecifier;
            funcNode->isConst = declInfo.isConst;
            funcNode->isStatic = declInfo.isStatic;
            funcNode->isVolatile = declInfo.isVolatile;
            funcNode->isUnsigned = declInfo.isUnsigned;
            funcNode->typeCategory = 3;
            
            // If this is a function definition (not just forward decl), reset param info
            // The function body will re-populate it
            bool hadForwardDecl = (existingFunc && existingFunc->typeCategory == 3);
            int expectedParamCount = funcNode->paramCount;  // Save for validation
            if (hadForwardDecl) {
                // Don't reset - keep the forward declaration's parameter info
                // We'll just validate that the definition matches
            }
            
            if(actualDeclarator->children.size() > 1 && actualDeclarator->children[1]->type == NODE_PARAMETER_LIST) {
            int definitionParamCount = 0;
            for (auto param : actualDeclarator->children[1]->children) {
                if (param->type == NODE_PARAMETER_DECLARATION) {

                    TreeNode* paramDeclarator = param->children[1];
                    TreeNode* varNode = paramDeclarator;
                    bool isRefParam = false;
                    int pointerDepth = 0;
                    bool isArrayParam = false;
                    
                    // Check for pointer (with or without reference)
                    if (paramDeclarator->type == NODE_POINTER) {
                        TreeNode* temp = paramDeclarator;
                        while (temp && temp->type == NODE_POINTER) {
                            pointerDepth++;
                            if (temp->children.empty()) break;
                            temp = temp->children[0];
                        }
                        // Check if we have a reference after the pointer(s)
                        if (temp && temp->type == NODE_REFERENCE) {
                            isRefParam = true;
                            if (!temp->children.empty()) {
                                varNode = temp->children[0];
                            }
                        } 
                        // Check if we have an array after the pointer(s)
                        else if (temp && temp->type == ARRAY) {
                            isArrayParam = true;
                            varNode = temp->children[0];
                        }
                        else {
                            varNode = temp ? temp : paramDeclarator;
                        }
                    }
                    // Check if parameter is a reference (no pointer)
                    else if (paramDeclarator->type == NODE_REFERENCE) {
                        isRefParam = true;
                        if (!paramDeclarator->children.empty()) {
                            varNode = paramDeclarator->children[0];
                        }
                    }
                    // Check if parameter is an array
                    else if (paramDeclarator->type == ARRAY) {
                        isArrayParam = true;
                        varNode = paramDeclarator->children[0];
                    }
                    
                    string varName = varNode->valueToString();
                    
                    bool isDuplicate = false;
                    for (const auto &entry : currentTable->symbolTable)
                    {
                        if (entry.first == varName)
                        {
                            cerr << "Error: Duplicate declaration of variable '" << varName << "'\n";
                            isDuplicate = true;
                            break;
                        }
                    }
                    if (isDuplicate) continue;

                    DeclaratorInfo paramInfo = isValidVariableDeclaration(param->children[0]->children, false);
                    if (paramInfo.isValid) {
                        // Check for invalid static parameter
                        if (paramInfo.isStatic) {
                            cerr << "Error: Function parameters cannot have 'static' storage class" << endl;
                            YYABORT;
                        }
                        
                        // Calculate total pointer level: typedef pointer level + declarator pointer level
                        int totalPointerLevel = paramInfo.pointerLevel + pointerDepth;
                        
                        if (isRefParam) {
                            varNode->typeCategory = 8;  // Reference
                            varNode->isReference = true;
                            varNode->pointerLevel = totalPointerLevel;  // Set pointer level for ref to pointer
                        } else if (isArrayParam) {
                            // Arrays as parameters are treated as pointers in C
                            varNode->typeCategory = 1;  // Pointer
                            varNode->pointerLevel = 1 + paramInfo.pointerLevel;  // Add typedef pointer level
                        } else if (totalPointerLevel > 0) {
                            varNode->typeCategory = 1;  // Pointer
                            varNode->pointerLevel = totalPointerLevel;
                        } else {
                            varNode->typeCategory = 0;  // Regular variable
                        }
                        varNode->storageClass = paramInfo.storageClass;
                        varNode->typeSpecifier = paramInfo.typeSpecifier;
                        varNode->isConst = paramInfo.isConst;
                        varNode->isStatic = paramInfo.isStatic;
                        varNode->isVolatile = paramInfo.isVolatile;
                        varNode->isUnsigned = paramInfo.isUnsigned;
                        
                        // If this is a struct parameter (including typedef struct), copy the symbol table
                        if (paramInfo.typeSpecifier == 20 && paramInfo.isCustomType) {
                            // Look up the struct definition to get its symbol table
                            TreeNode* typeNode = nullptr;
                            for (auto child : param->children[0]->children) {
                                if (child->type == NODE_TYPE_SPECIFIER) {
                                    string typeName = child->valueToString();
                                    // Check if it's a typedef name
                                    extern unordered_set<string> typedefNames;
                                    if (typedefNames.count(typeName) > 0) {
                                        typeNode = lookupSymbol(typeName, true);
                                    } else if (classOrStructOrUnion.count(typeName)) {
                                        typeNode = lookupSymbol(typeName, true);
                                    }
                                    // Handle wrapped NODE_TYPE_SPECIFIER (e.g., from struct_or_union_specifier)
                                    if (!typeNode && child->children.size() >= 2) {
                                        string possibleTypeName = child->children[1]->valueToString();
                                        if (classOrStructOrUnion.count(possibleTypeName)) {
                                            typeNode = lookupSymbol(possibleTypeName, true);
                                        }
                                    }
                                    if (typeNode && typeNode->symbolTable.size() > 0) {
                                        varNode->symbolTable = typeNode->symbolTable;
                                        break;
                                    }
                                }
                            }
                        }
                        
                        // Don't modify funcNode->paramTypes or paramCount here
                        // If there was a forward declaration, addDeclarators already set these
                        // If there wasn't, we need to add them
                        if (!existingFunc || existingFunc->paramCount == 0) {
                            funcNode->paramTypes.push_back(varNode->typeSpecifier);
                            funcNode->paramCount++;
                        }
                        insertSymbol(varName, varNode);
                        paramNames.push_back(varName);
                    }
                }
            }}
            
            // Emit 3AC to copy parameters from argument positions to local variables
            // This will be handled by MIPS generator to copy from $a0-$a3 registers
            for (size_t i = 0; i < paramNames.size(); i++) {
                string paramReg = "param" + to_string(i);  // Special marker for MIPS generator
                codeGen.emit(TACOp::ASSIGN, paramNames[i], paramReg, nullopt);
            }
        }else {
            cerr << "Error: Invalid function declaration for '" << $2->children[0]->valueToString() << "'\n";
        }
        inFunc = true;
    } compound_statement {
        // Check for unresolved forward gotos before exiting function
        if(!forwardGotoRefs.empty()) {
            cerr << "Error: Goto to undefined label(s): ";
            bool first = true;
            for(const auto& pair : forwardGotoRefs) {
                if(!first) cerr << ", ";
                cerr << "'" << pair.first << "'";
                first = false;
            }
            cerr << endl;
            YYABORT;
        }
        
        $$ = createNode(NODE_FUNCTION_DEFINITION, monostate(), $1, $2, $4);
        exitScope();
        inFunc = false;
        expectedReturnType = -1;
        
        // Clear label and goto data structures for next function
        Label_defn.clear();
        forwardGotoRefs.clear();
    }


destructor_function
    : BITWISE_NOT_OPERATOR ID LPAREN RPAREN {enterScope();} compound_statement {
        $$ = createNode(NODE_DESTRUCTOR_FUNCTION, monostate(),$2, $6); exitScope();
    }
    ;
%%

void yyerror(const char *s) {
    extern char *yytext;
    extern int yylineno;
    cout << "Error: " << s << " at '" << yytext << "' on line " << yylineno << endl;
}


int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        cout << "Error opening file" << endl;
        return 1;
    }

    currentTable = new Table();
    tableStack.push(currentTable);
    offsetStack.push(0);
    allTables.push_back(currentTable);

    // Add built-in library functions to global scope
    // malloc: void* malloc(size_t size)
    TreeNode* mallocNode = new TreeNode(NODE_IDENTIFIER, string("malloc"));
    mallocNode->typeCategory = 3;  // Function
    mallocNode->typeSpecifier = 0; // void* (treated as void, pointerLevel = 1)
    mallocNode->pointerLevel = 1;
    mallocNode->paramCount = 1;
    mallocNode->paramTypes.push_back(3);  // int (size_t ~ unsigned long, but we'll use int)
    mallocNode->paramPointerLevels.push_back(0);  // Not a pointer
    insertSymbol("malloc", mallocNode);
    
    // calloc: void* calloc(size_t num, size_t size)
    TreeNode* callocNode = new TreeNode(NODE_IDENTIFIER, string("calloc"));
    callocNode->typeCategory = 3;  // Function
    callocNode->typeSpecifier = 0; // void*
    callocNode->pointerLevel = 1;
    callocNode->paramCount = 2;
    callocNode->paramTypes.push_back(3);  // int
    callocNode->paramTypes.push_back(3);  // int
    callocNode->paramPointerLevels.push_back(0);  // Not a pointer
    callocNode->paramPointerLevels.push_back(0);  // Not a pointer
    insertSymbol("calloc", callocNode);
    
    // realloc: void* realloc(void* ptr, size_t size)
    TreeNode* reallocNode = new TreeNode(NODE_IDENTIFIER, string("realloc"));
    reallocNode->typeCategory = 3;  // Function
    reallocNode->typeSpecifier = 0; // void*
    reallocNode->pointerLevel = 1;
    reallocNode->paramCount = 2;
    reallocNode->paramTypes.push_back(3);  // int* (void* ptr - we treat as int* for compatibility)
    reallocNode->paramTypes.push_back(3);  // int (size)
    reallocNode->paramPointerLevels.push_back(1);  // First param is pointer
    reallocNode->paramPointerLevels.push_back(0);  // Second param is not pointer
    insertSymbol("realloc", reallocNode);
    
    // free: void free(void* ptr)
    TreeNode* freeNode = new TreeNode(NODE_IDENTIFIER, string("free"));
    freeNode->typeCategory = 3;  // Function
    freeNode->typeSpecifier = 0; // void
    freeNode->pointerLevel = 0;
    freeNode->paramCount = 1;
    freeNode->paramTypes.push_back(3);  // int* (void* ptr)
    freeNode->paramPointerLevels.push_back(1);  // Param is pointer
    insertSymbol("free", freeNode);
    
    // sizeof is handled as a unary operator, not as a function

    int result = yyparse();
    fclose(yyin);
    
    // If parsing failed, exit with error code
    if (result != 0) {
        cerr << "Compilation failed due to errors." << endl;
        return 1;
    }
    
    // Print symbol table for debugging (to stderr so it doesn't mix with 3AC output)
    streambuf* cerrBuf = cerr.rdbuf();
    printSymbolTables();
    cerr.rdbuf(cerrBuf);

    mkdir("output", 0777); 

    string inputPath(argv[1]);
    string base = inputPath.substr(inputPath.find_last_of("/\\") + 1);
    string outName = "output/" + base.substr(0, base.find_last_of('.')) + ".3ac";

    ofstream out(outName); 
    if (!out) {
        cerr << "Error opening output file: " << outName << endl;
        return 1;
    }

        streambuf* coutbuf = cout.rdbuf();
        cout.rdbuf(out.rdbuf());

        // Print static initialization section if there are any static variables
        if (!staticInitCode.empty()) {
            cout << ".static_init:\n";
            // Track which temporaries were resolved so we can skip them later
            unordered_set<string> resolvedTemps;
            
            for (size_t si = 0; si < staticInitCode.size(); ++si) {
                string init = staticInitCode[si];
                // Try to resolve temporaries in static init by looking in TAC
                // If init is "var = tX", find what tX equals and use that value directly
                string resolved = init;
                size_t eqPos = init.find(" = ");
                if (eqPos != string::npos) {
                    string rhs = init.substr(eqPos + 3);
                    if (rhs.size() > 0 && rhs[0] == 't') {
                        // This is a temporary - search the entire TAC for its definition
                        for (size_t i = 0; i < codeGen.tacCode.size(); i++) {
                            if (codeGen.tacCode[i].op == TACOp::ASSIGN &&
                                codeGen.tacCode[i].result == rhs &&
                                codeGen.tacCode[i].operand1.has_value() &&
                                !codeGen.tacCode[i].operand2.has_value()) {
                                // Found the assignment - use the value directly
                                resolved = init.substr(0, eqPos + 3) + codeGen.tacCode[i].operand1.value();
                                resolvedTemps.insert(rhs);  // Mark this temp as resolved
                                break;
                            }
                        }
                    }
                }
                // Update the staticInitCode entry so later code (MIPS generation) sees resolved values
                staticInitCode[si] = resolved;
                cout << resolved << "\n";
            }
            cout << "\n";
            
            // Remove resolved temporaries from TAC
            codeGen.tacCode.erase(
                remove_if(codeGen.tacCode.begin(), codeGen.tacCode.end(),
                    [&resolvedTemps](const TACInstruction& instr) {
                        return resolvedTemps.count(instr.result) > 0 &&
                               instr.op == TACOp::ASSIGN &&
                               !instr.operand2.has_value();
                    }),
                codeGen.tacCode.end()
            );
        }

        codeGen.printTAC();

        cout.rdbuf(coutbuf);

    // Generate MIPS assembly code
    string mipsOutName = "output/" + base.substr(0, base.find_last_of('.')) + ".asm";
    
    cout << "\n" << endl;
    generateMIPSFromTAC(mipsOutName);
    
    cout << "\nCompilation completed successfully!" << endl;
    cout << "3AC output: " << outName << endl;
    cout << "MIPS output: " << mipsOutName << endl;

    return 0;
}

// MIPS generation function implementation
void generateMIPSFromTAC(const string& outputFilename) {
    cout << "\n========================================" << endl;
    cout << "MIPS CODE GENERATION" << endl;
    cout << "========================================\n" << endl;
    
    // Create MIPS generator
    MIPSGenerator mipsGen;
    
    // Generate MIPS from 3AC
    cout << "Generating MIPS assembly from " << codeGen.tacCode.size() << " 3AC instructions..." << endl;
    mipsGen.generateMIPS(codeGen.tacCode);
    
    // Write to file
    cout << "Writing MIPS code to: " << outputFilename << endl;
    mipsGen.writeToFile(outputFilename);
    
    cout << "\nMIPS code generation completed successfully!" << endl;
    cout << "========================================\n" << endl;
}