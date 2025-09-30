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
            cerr << "Incompatible Type: " << $1->typeSpecifier << " and " 
                 << $3->typeSpecifier << " at line " << yylineno << endl;
        }
        }
    }
	;
    
relational_expression
	: shift_expression { $$ = $1; }
	| relational_expression LESS_THAN_OPERATOR shift_expression {
        $$ = createNode(NODE_RELATIONAL_EXPRESSION, $2, $1, $3);
        backTrackRelExpr($$);
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
            codeGen.emit(TACOp::oth, "", nullopt, nullopt, true);
        } else {
            cerr << "Incompatible Type: " << $1->typeSpecifier << " and " 
                 << $3->typeSpecifier << " at line " << yylineno << endl;
        } 
    }
    }
	| relational_expression GREATER_THAN_OPERATOR shift_expression { 
        $$ = createNode(NODE_RELATIONAL_EXPRESSION, $2, $1, $3);
        backTrackRelExpr($$);
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
            codeGen.emit(TACOp::oth, "", nullopt, nullopt, true);
            $$->tacResult = codeGen.newTemp();
        } else {
            cerr << "Incompatible Type: " << $1->typeSpecifier << " and " 
                 << $3->typeSpecifier << " at line " << yylineno << endl;
        } 
    }
    }
	| relational_expression LESS_THAN_OR_EQUAL_OPERATOR shift_expression {
        $$ = createNode(NODE_RELATIONAL_EXPRESSION, $2, $1, $3);
        backTrackRelExpr($$);
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
            codeGen.emit(TACOp::oth, "", nullopt, nullopt, true);
            $$->tacResult = codeGen.newTemp();
        } else {
            cerr << "Incompatible Type: " << $1->typeSpecifier << " and " 
                 << $3->typeSpecifier << " at line " << yylineno << endl;
        }
    }
    }
	| relational_expression GREATER_THAN_OR_EQUAL_OPERATOR shift_expression { 
        $$ = createNode(NODE_RELATIONAL_EXPRESSION, $2, $1, $3);
        $$->isLValue = false;
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        backTrackRelExpr($$);
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
            codeGen.emit(TACOp::oth, "", nullopt, nullopt, true);
            $$->tacResult = codeGen.newTemp();
        } else {
            cerr << "Incompatible Type: " << $1->typeSpecifier << " and " 
                 << $3->typeSpecifier << " at line " << yylineno << endl;
        } 
    }
    }
	;

equality_expression
	: relational_expression { $$ = $1; }
	| equality_expression EQUALS_COMPARISON_OPERATOR relational_expression { 
        $$ = createNode(NODE_EQUALITY_EXPRESSION, $2, $1, $3);
        $$->isLValue = false;
        backTrackRelExpr($$);
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(rhsPointerLevel!=lhsPointerLevel){
                cerr << "Incompatible types: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
        if (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, "==")) {
            $$->typeSpecifier = 3;
            if($1->typeSpecifier != $3->typeSpecifier){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                $3->tacResult = temp;
            }
            codeGen.emit(TACOp::EQ, "", $1->tacResult, $3->tacResult, true);
            codeGen.emit(TACOp::oth, "", nullopt, nullopt, true);
            $$->tacResult = codeGen.newTemp();
        } else {
            cerr << "Incompatible Type: " << $1->typeSpecifier << " and " 
                 << $3->typeSpecifier << " at line " << yylineno << endl;
        } 
    }
    }
	| equality_expression NOT_EQUALS_OPERATOR relational_expression { 
        $$ = createNode(NODE_EQUALITY_EXPRESSION, $2, $1, $3);
        $$->isLValue = false;
        backTrackRelExpr($$);
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(rhsPointerLevel!=lhsPointerLevel){
                cerr << "Incompatible types: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
        if (isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, "!=")) {
            $$->typeSpecifier = 3;
            if($1->typeSpecifier != $3->typeSpecifier){
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::TYPECAST, temp,typeCastInfo($1->typeSpecifier, $3->typeSpecifier) , $3->tacResult);
                $3->tacResult = temp;
            }
            codeGen.emit(TACOp::NE, "", $1->tacResult, $3->tacResult, true);
            codeGen.emit(TACOp::oth, "", nullopt, nullopt, true);
            $$->tacResult = codeGen.newTemp();
        } else {
            cerr << "Incompatible Type: " << $1->typeSpecifier << " and " 
                 << $3->typeSpecifier << " at line " << yylineno << endl;
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
            cerr << "Incompatible Type: " << $1->typeSpecifier << " and " 
                 << $3->typeSpecifier << " at line " << yylineno << endl;
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
            cerr << "Incompatible Type: " << $1->typeSpecifier << " and " 
                 << $3->typeSpecifier << " at line " << yylineno << endl;
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
            cerr << "Incompatible Type: " << $1->typeSpecifier << " and " 
                 << $3->typeSpecifier << " at line " << yylineno << endl;
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
            if(lookupSymbol($1->tacResult, true)){
            backpatchNode* curr = $1->trueList;
            backpatchNode* next = Backpatch::addToBackpatchList(curr, codeGen.currentInstrIndex);
            $1->trueList = next;
            curr = $1->falseList;
            next = Backpatch::addToBackpatchList(curr, codeGen.currentInstrIndex + 1);
            $1->falseList = next;
            codeGen.emit(TACOp::NE, "", $1->tacResult, "0", true);
            codeGen.emit(TACOp::oth, "", nullopt, nullopt, true);
        }

        if(lookupSymbol($4->tacResult, true)){
            backpatchNode* curr = $4->trueList;
            backpatchNode* next = Backpatch::addToBackpatchList(curr, codeGen.currentInstrIndex);
            $4->trueList = next;
            curr = $4->falseList;
            next = Backpatch::addToBackpatchList(curr, codeGen.currentInstrIndex + 1);
            $4->falseList = next;
            codeGen.emit(TACOp::NE, "", $4->tacResult, "0", true);
            codeGen.emit(TACOp::oth, "", nullopt, nullopt, true);
        }
            string temp = codeGen.newTemp();
            $$->tacResult = temp; 
            Backpatch::backpatch($1->trueList, $3->tacResult);
            $$->trueList = $4->trueList;
            $$->falseList = Backpatch::mergeBackpatchLists($1->falseList, $4->falseList);
        } else {
            cerr << "Incompatible Type: " << $1->typeSpecifier << " and " 
                 << $3->typeSpecifier << " at line " << yylineno << endl;
        }
        $$->tacResult = codeGen.newLabel();
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
                $3->tacResult = temp;
            }
            if(lookupSymbol($1->tacResult, true)){
                backpatchNode* curr = $1->trueList;
                backpatchNode* next = Backpatch::addToBackpatchList(curr, codeGen.currentInstrIndex);
                $1->trueList = next;
                curr = $1->falseList;
                next = Backpatch::addToBackpatchList(curr, codeGen.currentInstrIndex + 1);
                $1->falseList = next;
                codeGen.emit(TACOp::NE, "", $1->tacResult, "0", true);
                codeGen.emit(TACOp::oth, "", nullopt, nullopt, true);
            }

        if(lookupSymbol($4->tacResult, true)){
                backpatchNode* curr = $4->trueList;
                backpatchNode* next = Backpatch::addToBackpatchList(curr, codeGen.currentInstrIndex);
                $4->trueList = next;
                curr = $4->falseList;
                next = Backpatch::addToBackpatchList(curr, codeGen.currentInstrIndex + 1);
                $4->falseList = next;
                codeGen.emit(TACOp::NE, "", $4->tacResult, "0", true);
                codeGen.emit(TACOp::oth, "", nullopt, nullopt, true);
            }
            string temp = codeGen.newTemp();
            $$->tacResult = temp; 
            Backpatch::backpatch($1->falseList, $3->tacResult);
            $$->falseList = $4->falseList;
            $$->trueList = Backpatch::mergeBackpatchLists($1->trueList, $4->trueList);
        } else {
            cerr << "Incompatible Type: " << $1->typeSpecifier << " and " 
                 << $3->typeSpecifier << " at line " << yylineno << endl;
        }
    }
	;

M
  : {
      $$ = new TreeNode(OTHERS);
      $$->tacResult = to_string(codeGen.currentInstrIndex + 1);
    }


conditional_expression
    : logical_or_expression { 
        $$ = $1; 
    }
    | logical_or_expression TERNARY_OPERATOR expression COLON conditional_expression { 
        int rhsPointerLevel = $1->pointerLevel;
        int lhsPointerLevel = $3->pointerLevel;
        if(!rhsPointerLevel || !lhsPointerLevel){
                cerr << "Incompatible types: " 
                     << $1->typeSpecifier << " and " << $3->typeSpecifier 
                     << " at line " << yylineno << endl;
        }
        else{
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
        }
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
        
        if(rhsPointerLevel!=lhsPointerLevel && !($1->typeSpecifier == 3 && $3->typeCategory == 2)){
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
            
            if (isStructAssignment || isTypeCompatible($1->typeSpecifier, $3->typeSpecifier, op)) {
                $$->typeSpecifier = $1->typeSpecifier;
                if($1->typeSpecifier != $3->typeSpecifier){
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
        //     Backpatch::backpatch($3->trueList, to_string(codeGen.currentInstrIndex + 1));
        //     Backpatch::backpatch($3->falseList, to_string(codeGen.currentInstrIndex + 2));
        //     handleAssignment($2->valueToString(), $1->tacResult, "1", true);
        //     handleAssignment($2->valueToString(), $1->tacResult, "0", true);
        // } else {
        //     handleAssignment($2->valueToString(), $1->tacResult, $3->tacResult);
        // }
        string temp = codeGen.newTemp();
        if($2->valueToString() == "="){
            codeGen.emit(TACOp::ASSIGN, $1->tacResult, $3->tacResult, nullopt);
        }else if($2->valueToString() == "+="){
            codeGen.emit(TACOp::ADD, $1->tacResult, $1->tacResult, $3->tacResult);
        }else if($2->valueToString() == "-="){
            codeGen.emit(TACOp::SUB, $1->tacResult, $1->tacResult, $3->tacResult);
        }else if($2->valueToString() == "*="){
            codeGen.emit(TACOp::MUL, $1->tacResult, $1->tacResult, $3->tacResult);
        }else if($2->valueToString() == "/="){
            codeGen.emit(TACOp::DIV, $1->tacResult, $1->tacResult, $3->tacResult);
        }else if($2->valueToString() == "%="){
            codeGen.emit(TACOp::MOD, $1->tacResult, $1->tacResult, $3->tacResult);
        }else if($2->valueToString() == "&="){
            codeGen.emit(TACOp::BIT_AND, $1->tacResult, $1->tacResult, $3->tacResult);
        }else if($2->valueToString() == "|="){
            codeGen.emit(TACOp::BIT_OR, $1->tacResult, $1->tacResult, $3->tacResult);
        }else if($2->valueToString() == "^="){
            codeGen.emit(TACOp::BIT_XOR, $1->tacResult, $1->tacResult, $3->tacResult);
        }else if($2->valueToString() == "<<="){
            codeGen.emit(TACOp::LSHFT,$1->tacResult,$1->tacResult,$3->tacResult);
        }else if($2->valueToString() == ">>="){
            codeGen.emit(TACOp::RSHFT,$1->tacResult,$1->tacResult,$3->tacResult);
        }
        $$->tacResult = temp;
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
        if(lookupSymbol($$->tacResult, true)){
            backpatchNode* curr = $1->trueList;
            backpatchNode* next = Backpatch::addToBackpatchList(curr, codeGen.currentInstrIndex);
            $$->trueList = next;
            curr = $1->falseList;
            next = Backpatch::addToBackpatchList(curr, codeGen.currentInstrIndex + 1);
            $$->falseList = next;
            codeGen.emit(TACOp::NE, "", $1->tacResult, "0", true);
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
	| type_specifier { $$ = createNode(NODE_DECLARATION_SPECIFIERS, monostate(), $1);$$->storageClass = $1->storageClass;}
	| type_specifier declaration_specifiers { 
        $$ = createNode(NODE_DECLARATION_SPECIFIERS, monostate(), $1);
        for(auto child : $2->children){
            $$->addChild(child);
        }
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
    | KEYWORD_FLOAT { $$ = $1; $$->type = NODE_TYPE_SPECIFIER; $$->storageClass = 12;}
    | KEYWORD_DOUBLE { $$ = $1; $$->type = NODE_TYPE_SPECIFIER; $$->storageClass = 13;}
    | KEYWORD_SIGNED { $$ = $1; $$->type = NODE_TYPE_SPECIFIER; }
    | KEYWORD_UNSIGNED { $$ = $1; $$->type = NODE_TYPE_SPECIFIER; }
    | TYPE_NAME {$$ = $1; $$->type = NODE_TYPE_SPECIFIER; }
    | TYPEDEF_NAME {$$ = $1; $$->type = NODE_TYPE_SPECIFIER; }
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
            if(pointerLevel == 1){node->typeSpecifier *= 10;}
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
        Backpatch::backpatch($1->nextList, to_string(codeGen.currentInstrIndex + 1)); 
        }
	| iteration_statement { 
        $$ = $1;
        Backpatch::backpatch($1->nextList, to_string(codeGen.currentInstrIndex  + 1)); 
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
                cerr << "Error: Type mismatch between format specifiers and arguments in printf" << endl;
                YYABORT;
            }else{
                codeGen.emit(TACOp::ASSIGN, "param", $3->tacResult);
                for (auto* arg : $5->children) {
                    codeGen.emit(TACOp::ASSIGN, "param", arg->tacResult);
                }
                string temp = codeGen.newTemp();
                codeGen.emit(TACOp::CALL, temp, "printf", to_string($5->children.size() + 1));
                $$->tacResult = temp;   
            }
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
            Label_defn[$1->valueToString()] = to_string(codeGen.currentInstrIndex + 1);
        }
    } COLON statement { 
        $$ = createNode(NODE_LABELED_STATEMENT, monostate(), $1, $4);
        $1->typeCategory = 7;
        $1->typeSpecifier = 9;
        insertSymbol($1->valueToString(), $1);
        }
    | KEYWORD_CASE constant_expression COLON {
        if(inSwitch.size() == 0) {
            cerr << "Error: Case label must be inside switch statement" << endl;
            YYABORT;
        }
        
        // Validate that the case expression is a constant (literal)
        // Check if it's a primary expression with a literal value
        string caseValue = $2->valueToString();
        
        // Check if the value is a variable by looking it up in symbol table
        // If it's found as a variable, it's not a constant
        if(lookupSymbol(caseValue, true) != nullptr) {
            cerr << "Error: Case label must be a constant expression, not a variable: " << caseValue << endl;
            YYABORT;
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
    }
	;


compound_statement
    : LBRACE { enterScope(); } RBRACE { $$ = createNode(NODE_COMPOUND_STATEMENT, monostate()); exitScope();}
    | LBRACE { enterScope(); } block_item_list RBRACE {$$ = $3; exitScope(); }
    ;

block_item_list
    : block_item { 
        $$ = createNode(NODE_BLOCK_ITEM_LIST, monostate(), $1);
        $$->continueList = $1->continueList;
        $$->breakList = $1->breakList; 
    }
    | block_item_list block_item { 
        $$ = $1; 
        $$->children.push_back($2);
        $$->continueList = Backpatch::mergeBackpatchLists($1->continueList, $2->continueList);
        $$->breakList = Backpatch::mergeBackpatchLists($1->breakList, $2->breakList);
        }
    ;

block_item
    : declaration 
    | statement {$$ = $1;}
    ;

expression_statement
	: SEMICOLON { $$ = createNode(NODE_EXPRESSION_STATEMENT, monostate()); }
	| expression SEMICOLON { $$ = $1; }
	;


selection_statement
    :KEYWORD_IF LPAREN single_expression RPAREN M statement N %prec NO_ELSE  {
        Backpatch::backpatch($3->trueList, $5->tacResult); 
        backpatchNode* merged = Backpatch::mergeBackpatchLists($3->falseList, $7->nextList); 
        $$->nextList = Backpatch::mergeBackpatchLists(merged, $6->nextList);                
    }
    | KEYWORD_IF LPAREN single_expression RPAREN M statement N KEYWORD_ELSE M statement  {
        Backpatch::backpatch($3->trueList, $5->tacResult); 
        Backpatch::backpatch($3->falseList, $9->tacResult);
        backpatchNode* merged = Backpatch::mergeBackpatchLists($6->nextList, $7->nextList);
        $$->nextList = Backpatch::mergeBackpatchLists(merged, $10->nextList);
    }
    | KEYWORD_SWITCH LPAREN expression RPAREN {
        inSwitch.push(false);
        switchExpr.push($3->tacResult); // Store the switch expression
        switchCases.push(vector<pair<string, string>>()); // Initialize empty case list
        switchDefaultLabel.push(""); // Initialize empty default label
        
        // Save the position where we need to insert case comparisons (after expression evaluation)
        $<intVal>$ = codeGen.currentInstrIndex;
    } statement {
        $$ = createNode(NODE_SELECTION_STATEMENT, monostate(), $3, $6);
        
        // Get the saved information
        int insertPos = $<intVal>5;
        string expr = switchExpr.top();
        vector<pair<string, string>> cases = switchCases.top();
        string defaultLabel = switchDefaultLabel.top();
        
        // Generate end label FIRST (before inserting, so backpatch indices are correct relative to current code)
        string endLabel = codeGen.newLabel();
        codeGen.emit(TACOp::LABEL, endLabel, nullopt, nullopt);
        
        // Backpatch all break statements to the end of switch (do this BEFORE inserting)
        Backpatch::backpatch($6->breakList, endLabel);
        
        // Now insert comparison code at the saved position
        // This will shift all subsequent instructions (including the backpatched ones)
        // This is a limitation - the backpatched indices will be wrong after insertion
        int currentInsertPos = insertPos;
        
        // Generate comparisons for all cases
        for(const auto& caseInfo : cases) {
            string caseValue = caseInfo.first;
            string caseLabel = caseInfo.second;
            
            // if (expr == caseValue) goto caseLabel
            codeGen.insertAt(currentInsertPos++, TACOp::IF_EQ, caseLabel, expr, caseValue);
        }
        
        // After all comparisons, jump to default or end
        if(!defaultLabel.empty()) {
            codeGen.insertAt(currentInsertPos++, TACOp::GOTO, defaultLabel, nullopt, nullopt);
        } else {
            codeGen.insertAt(currentInsertPos++, TACOp::GOTO, endLabel, nullopt, nullopt);
        }
        
        // Clean up stacks
        inSwitch.pop();
        switchExpr.pop();
        switchCases.pop();
        switchDefaultLabel.pop();
    }
    
iteration_statement
    : M KEYWORD_WHILE LPAREN single_expression RPAREN {enterScope(); inLoop++;} M statement  {
        Backpatch::backpatch($4->trueList, $7->tacResult);
        Backpatch::backpatch($8->nextList, $1->tacResult);
        codeGen.emit(TACOp::oth, $1->tacResult, nullopt, nullopt, true);
        Backpatch::backpatch($8->continueList, $1->tacResult);
        $$->nextList = Backpatch::mergeBackpatchLists($8->breakList, $4->falseList);
        inLoop--;
        exitScope();
    }
    | M KEYWORD_UNTIL LPAREN single_expression RPAREN {enterScope(); inLoop++;} M statement  {
        // until is like while(!(condition)), so swap true and false lists
        Backpatch::backpatch($4->falseList, $7->tacResult);
        Backpatch::backpatch($8->nextList, $1->tacResult);
        codeGen.emit(TACOp::oth, $1->tacResult, nullopt, nullopt, true);
        Backpatch::backpatch($8->continueList, $1->tacResult);
        $$->nextList = Backpatch::mergeBackpatchLists($8->breakList, $4->trueList);
        inLoop--;
        exitScope();
    }
    | KEYWORD_DO M {enterScope(); inLoop++;} statement M  {inLoop--; exitScope();} KEYWORD_WHILE LPAREN single_expression RPAREN{
        Backpatch::backpatch($9->trueList, $2->tacResult);
        Backpatch::backpatch($4->continueList, $5->tacResult);
        $$->nextList = Backpatch::mergeBackpatchLists($4->breakList, $9->falseList);
    }
    | KEYWORD_FOR LPAREN {enterScope();} for_init M for_cond M for_inc RPAREN {
            inLoop++;
            codeGen.emit(TACOp::oth, $5->tacResult, nullopt, nullopt, true);
        } M statement { 
            Backpatch::backpatch($6->trueList, $11->tacResult);            
            Backpatch::backpatch($12->nextList, $7->tacResult);
            Backpatch::backpatch($12->continueList, $7->tacResult);
            $$->nextList = Backpatch::mergeBackpatchLists($12->breakList, $6->falseList);
            codeGen.emit(TACOp::oth, $7->tacResult, nullopt, nullopt, true);
            exitScope();
            inLoop--; 
        }
    ;

for_init
    : expression_statement {$$ = $1;}
    | declaration {$$ = $1;}
    ;
for_cond
    : expression_statement {$$ = $1; }
    ;

for_inc
    : /* empty */ { $$ = createNode(NODE_EXPRESSION_STATEMENT, monostate()); }
    | expression {$$ = $1;}
    | expression_statement {$$ =$1;} 
    ;

jump_statement
	: KEYWORD_GOTO ID SEMICOLON { 
        $$ = createNode(NODE_JUMP_STATEMENT, monostate(), $1, $2);
        if(Label_defn.count($2->valueToString()) == 0){
            cerr << "Error: Goto to undefined label '" << $2->valueToString() << "'" << endl;
            YYABORT;
        }else{
            codeGen.emit(TACOp::GOTO, Label_defn[$2->valueToString()]);
        }
    }
	| KEYWORD_CONTINUE SEMICOLON {
        if(inLoop <= 0){
            cerr << "Error: Continue statement must be inside a loop" << endl;
            YYABORT;
        }
        // Note: Continue is allowed in a loop inside a switch
        $$ = $1;
        backpatchNode* curr = nullptr;
        backpatchNode* next = Backpatch::addToBackpatchList(curr, codeGen.currentInstrIndex);
        $$->continueList = next;
        codeGen.emit(TACOp::oth, "", nullopt, nullopt, true);
    }
	| KEYWORD_BREAK SEMICOLON { 
        if(inLoop <= 0 && inSwitch.size() == 0){
            cerr << "Error: Break statement must be inside a loop or switch statement" << endl;
            YYABORT;
        }
        backpatchNode* curr = nullptr;
        backpatchNode* next = Backpatch::addToBackpatchList(curr, codeGen.currentInstrIndex);
        $$->breakList = next;
        codeGen.emit(TACOp::oth, "", nullopt, nullopt, true);
     }
	| KEYWORD_RETURN SEMICOLON { 
        if(!inFunc) {
            cerr << "Error: Return statement must be inside a function" << endl;
            YYABORT;
        }
        $$ = $1;
        if(expectedReturnType != -1){
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
        if(expectedReturnType == -1){
            cerr << "Error: Return statement without a value in void function.\n";
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
            
            string funcName = actualDeclarator->children[0]->valueToString();
            codeGen.emit(TACOp::LABEL, funcName, nullopt, nullopt);
            insertSymbol(funcName, actualDeclarator->children[0]);
            enterScope();
            TreeNode* funcNode = actualDeclarator->children[0];
            funcNode->storageClass = declInfo.storageClass;
            funcNode->typeSpecifier = declInfo.typeSpecifier;
            funcNode->isReference = isRefReturn;
            expectedReturnType = declInfo.typeSpecifier;
            funcNode->isConst = declInfo.isConst;
            funcNode->isStatic = declInfo.isStatic;
            funcNode->isVolatile = declInfo.isVolatile;
            funcNode->isUnsigned = declInfo.isUnsigned;
            funcNode->typeCategory = 3;
            if(actualDeclarator->children.size() > 1 && actualDeclarator->children[1]->type == NODE_PARAMETER_LIST) {
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
                        if (isRefParam) {
                            varNode->typeCategory = 8;  // Reference
                            varNode->isReference = true;
                            varNode->pointerLevel = pointerDepth;  // Set pointer level for ref to pointer
                        } else if (isArrayParam) {
                            // Arrays as parameters are treated as pointers in C
                            varNode->typeCategory = 1;  // Pointer
                            varNode->pointerLevel = 1;
                        } else if (pointerDepth > 0) {
                            varNode->typeCategory = 1;  // Pointer
                            varNode->pointerLevel = pointerDepth;
                        } else {
                            varNode->typeCategory = 0;  // Regular variable
                        }
                        varNode->storageClass = paramInfo.storageClass;
                        varNode->typeSpecifier = paramInfo.typeSpecifier;
                        varNode->isConst = paramInfo.isConst;
                        varNode->isStatic = paramInfo.isStatic;
                        varNode->isVolatile = paramInfo.isVolatile;
                        varNode->isUnsigned = paramInfo.isUnsigned;
                        funcNode->paramTypes.push_back(varNode->typeSpecifier);
                        funcNode->paramCount++;
                        insertSymbol(varName, varNode);
                    }
                }
            }}                    
        }else {
            cerr << "Error: Invalid function declaration for '" << $2->children[0]->valueToString() << "'\n";
        }
        inFunc = true;
    } compound_statement {
        $$ = createNode(NODE_FUNCTION_DEFINITION, monostate(), $1, $2, $4);
        exitScope();
        inFunc = false;
        expectedReturnType = -1;
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
    insertSymbol("malloc", mallocNode);
    
    // calloc: void* calloc(size_t num, size_t size)
    TreeNode* callocNode = new TreeNode(NODE_IDENTIFIER, string("calloc"));
    callocNode->typeCategory = 3;  // Function
    callocNode->typeSpecifier = 0; // void*
    callocNode->pointerLevel = 1;
    callocNode->paramCount = 2;
    callocNode->paramTypes.push_back(3);  // int
    callocNode->paramTypes.push_back(3);  // int
    insertSymbol("calloc", callocNode);
    
    // realloc: void* realloc(void* ptr, size_t size)
    TreeNode* reallocNode = new TreeNode(NODE_IDENTIFIER, string("realloc"));
    reallocNode->typeCategory = 3;  // Function
    reallocNode->typeSpecifier = 0; // void*
    reallocNode->pointerLevel = 1;
    reallocNode->paramCount = 2;
    reallocNode->paramTypes.push_back(0);  // void* (ptr)
    reallocNode->paramTypes.push_back(3);  // int (size)
    insertSymbol("realloc", reallocNode);
    
    // free: void free(void* ptr)
    TreeNode* freeNode = new TreeNode(NODE_IDENTIFIER, string("free"));
    freeNode->typeCategory = 3;  // Function
    freeNode->typeSpecifier = 0; // void
    freeNode->pointerLevel = 0;
    freeNode->paramCount = 1;
    freeNode->paramTypes.push_back(0);  // void* (ptr)
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
            for (const auto& init : staticInitCode) {
                cout << init << "\n";
            }
            cout << "\n";
        }

        codeGen.printTAC();

        cout.rdbuf(coutbuf);

    return 0;
}