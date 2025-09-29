%code requires {
	#include "../inc/treeNode.h"
    #include <bits/stdc++.h>
}
%define parse.error verbose

%{
    #include <bits/stdc++.h>
	#include "../inc/treeNode.h"
    #include "../inc/symbolTable.h" 
    #include <sys/stat.h> // for mkdir

    extern CodeGenerator codeGen;
    using namespace std;
    void yyerror(const char *s);

    extern int yylex();
    extern int yylineno;
    extern FILE *yyin;
    extern unordered_set<string> classOrStructOrUnion;
    extern unordered_set<string> typedefNames;
    extern int expectedReturnType;
    map<string,string> Label_defn;
    
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
        if(nd->trueList || nd->falseList){
            Backpatch::backpatch(nd->trueList, to_string(codeGen.currentInstrIndex  +1));
            Backpatch::backpatch(nd->falseList, to_string(codeGen.currentInstrIndex  + 2));
            codeGen.emit(TACOp::oth, nd->tacResult, "1", nullopt);
            codeGen.emit(TACOp::oth, nd->tacResult, "0", nullopt);
    }
}

void backTrackRelExpr(TreeNode* nd){
        if(nd == nullptr) return;
        backpatchNode* curr = nd->trueList;
        backpatchNode* next = Backpatch::addToBackpatchList(curr, codeGen.currentInstrIndex);
        nd->trueList = next;
        curr = nd->falseList;
        next = Backpatch::addToBackpatchList(curr, codeGen.currentInstrIndex + 1);
        nd->falseList = next;
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

%}


%union {
	TreeNode *node;
    char *str;
    int integer;
    int intVal;
}


%token <node>
    KEYWORD_AUTO KEYWORD_BREAK KEYWORD_CASE KEYWORD_CHAR 
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
    backpatchNode* curr = $$->nextList;
    backpatchNode* next = Backpatch::addToBackpatchList(curr, codeGen.currentInstrIndex);
    $$->nextList = next;
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
        codeGen.emit(TACOp::oth, "", nullopt, nullopt, true);
        if($1->valueToString() == "true"){
            backpatchNode* curr = $$->trueList;
            backpatchNode* next = Backpatch::addToBackpatchList(curr, codeGen.currentInstrIndex);
            $$->trueList = next;
        }else{
            backpatchNode* curr = $$->falseList;
            backpatchNode* next = Backpatch::addToBackpatchList(curr, codeGen.currentInstrIndex);
            $$->falseList = next;
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
            $$->typeCategory = 2;
            $$->isLValue = true;
            // Copy symbol table for array of structs
            if ($1->typeSpecifier == 20 && $1->symbolTable.size() > 0) {
                $$->symbolTable = $1->symbolTable;
            }
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::INDEX, temp, $1->tacResult, $3->tacResult);
            $$->tacResult = temp;
        } else if ($1->typeCategory == 1 || $1->pointerLevel > 0) {
            // Pointer indexing (including multi-level pointers like char**)
            $$->typeSpecifier = $1->typeSpecifier;
            $$->isLValue = true;
            
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
            
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::INDEX, temp, $1->tacResult, $3->tacResult);
            $$->tacResult = temp;
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
            $$->isLValue = false;
        } 
    }
    | postfix_expression LPAREN argument_expression_list RPAREN { 
        $$ = createNode(NODE_POSTFIX_EXPRESSION, monostate(), $1, $3); 
        $$->typeSpecifier = $1->typeSpecifier;
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
                        if(lhs != rhs){
                            string temp = codeGen.newTemp();
                            codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo(lhs, rhs) , $3->children[i]->tacResult);
                            $3->children[i]->tacResult = temp;
                        }
                    }
                }
            } else {
                cerr << "Error: function call with " << $3->children.size() << " params, expected " << $1->paramCount << endl;
                YYABORT;
            }
        }
        for (auto* arg : $3->children) {
            codeGen.emit(TACOp::ASSIGN, "param", arg->tacResult, nullopt);
        }
        if($1->typeSpecifier == 0){
            codeGen.emit(TACOp::CALL2, "", $1->valueToString(), to_string($3->children.size()));
        }else{
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::CALL, temp, $1->tacResult, to_string($3->children.size()));
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
                        
                        // Generate member access: structVar.member
                        $$->tacResult = $1->tacResult + "." + $3->valueToString();
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
                if (absDecl->type == NODE_ABSTRACT_DECLARATOR && !absDecl->children.empty()) {
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
        int size = 4;  // Default size
        
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
            codeGen.emit(TACOp::ASSIGN, temp, "&" + $2->tacResult, nullopt);
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
            codeGen.emit(TACOp::EQ, temp, $2->tacResult, "0");
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
            int toType = $2->typeSpecifier;
            int fromType = $4->typeSpecifier;

            if (!isValidCast(toType, fromType)) {
               cerr <<  "Invalid type cast" << endl;
            }
            
        $$ = createNode(NODE_CAST_EXPRESSION, monostate(), $2, $4);
            $$->typeSpecifier = toType;
            $$->isLValue = false;        string temp = codeGen.newTemp();
        string castExpr = "(" + $2->valueToString() + ")" + $4->tacResult;
        codeGen.emit(TACOp::ASSIGN, temp, castExpr, nullopt);
        $$->tacResult = temp; }
	;

multiplicative_expression
	: cast_expression { 
        $$ = $1; 
    }
	| multiplicative_expression MULTIPLY_OPERATOR cast_expression { 
        $$ = createNode(NODE_MULTIPLICATIVE_EXPRESSION, $2, $1, $3);
        backTrackExpr($1);
        backTrackExpr($3);
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
            cerr << "Incompatible Type: " << $1->typeSpecifier << " and " << $3->typeSpecifier 
                 << " at line " << yylineno << endl;
        }
        
        }
    }
	| multiplicative_expression DIVIDE_OPERATOR cast_expression { 
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
            cerr << "Incompatible Type: " << $1->typeSpecifier << " and " << $3->typeSpecifier 
                 << " at line " << yylineno << endl;
        }
        }
    }
	| multiplicative_expression MODULO_OPERATOR cast_expression { 
        $$ = createNode(NODE_MULTIPLICATIVE_EXPRESSION, $2, $1, $3);
        backTrackExpr($1);
        backTrackExpr($3);
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
            cerr << "Incompatible Type: " << $1->typeSpecifier << " and " << $3->typeSpecifier 
                 << " at line " << yylineno << endl;
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
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(rhsPointerLevel && lhsPointerLevel){
                cerr << "Error: Cannot add two pointers (invalid pointer arithmetic) at line " << yylineno << endl;
                YYABORT;
        }
        else{
    $$->isLValue = false; 
    $$->pointerLevel = lhsPointerLevel + rhsPointerLevel;
    if (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, "+")) {
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
        cerr << "Incompatible Type: " << $1->typeSpecifier << " and " 
             << $3->typeSpecifier << " at line " << yylineno << endl;
    } }

    }
	| additive_expression MINUS_OPERATOR multiplicative_expression { 
        $$ = createNode(NODE_ADDITIVE_EXPRESSION, $2, $1, $3);
        backTrackExpr($1);
        backTrackExpr($3);
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
        if (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, "-")) {
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
            cerr << "Incompatible Type: " << $1->typeSpecifier << " and " 
                 << $3->typeSpecifier << " at line " << yylineno << endl;
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
            cerr << "Incompatible Type: " << $1->typeSpecifier << " and " 
                 << $3->typeSpecifier << " at line " << yylineno << endl;
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
