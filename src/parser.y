%code requires {
	#include "../inc/ast.h"
}

%{
    #include <bits/stdc++.h>
	#include "../inc/ast.h"
    #include "../inc/symbolTable.h"
    using namespace std;
    
    void yyerror(const char *s);
    
    extern int yylex();
    extern FILE *yyin;
    
    // Set to track typedef names for lexer feedback
    set<string> typedefNames;
    
    // Function to register a typedef name
    void registerTypedefName(const string& name);

%}


%union {
	ASTNode *node;
    char *str;
}


/* Keywords */
%token <node>
    KEYWORD_AUTO KEYWORD_BOOL KEYWORD_BREAK KEYWORD_CASE KEYWORD_CATCH KEYWORD_CHAR 
    KEYWORD_CLASS KEYWORD_CONST KEYWORD_CONTINUE KEYWORD_DEFAULT KEYWORD_DELETE KEYWORD_DO 
    KEYWORD_DOUBLE KEYWORD_ELSE KEYWORD_EXTERN KEYWORD_FLOAT 
    KEYWORD_FOR KEYWORD_GOTO KEYWORD_IF KEYWORD_INT 
    KEYWORD_LONG KEYWORD_NEW KEYWORD_NULLPTR KEYWORD_PRIVATE KEYWORD_PROTECTED 
    KEYWORD_PUBLIC KEYWORD_REGISTER KEYWORD_RETURN KEYWORD_SHORT KEYWORD_SIGNED KEYWORD_SIZEOF 
    KEYWORD_STATIC KEYWORD_STRUCT KEYWORD_SWITCH KEYWORD_THIS KEYWORD_THROW  
    KEYWORD_TRY KEYWORD_TYPEDEF KEYWORD_UNSIGNED 
    KEYWORD_VOID KEYWORD_VOLATILE KEYWORD_WHILE KEYWORD_PRINTF KEYWORD_SCANF

/* Identifiers and Literals */
%token <node> INTEGER FLOAT CHAR STRING ID ELLIPSIS_OPERATOR BOOLEAN_LITERAL TYPEDEF_NAME

/* Delimiters and Separators */
%token <str>
    LBRACE RBRACE LBRACKET RBRACKET LPAREN RPAREN SEMICOLON COMMA COLON

/* Operators */
%token <str>
    PLUS_OPERATOR MINUS_OPERATOR MULTIPLY_OPERATOR DIVIDE_OPERATOR MODULO_OPERATOR
    DECREMENT_OPERATOR INCREMENT_OPERATOR

/* Assignment Operators */
%token <str>
    ASSIGNMENT_OPERATOR PLUS_ASSIGN_OPERATOR MINUS_ASSIGN_OPERATOR MULTIPLY_ASSIGN_OPERATOR 
    DIVIDE_ASSIGN_OPERATOR MODULO_ASSIGN_OPERATOR BITWISE_AND_ASSIGN_OPERATOR BITWISE_OR_ASSIGN_OPERATOR 
    BITWISE_XOR_ASSIGN_OPERATOR RIGHT_SHIFT_ASSIGN_OPERATOR LEFT_SHIFT_ASSIGN_OPERATOR

/* Comparison Operators */
%token <str>
    EQUALS_COMPARISON_OPERATOR NOT_EQUALS_OPERATOR GREATER_THAN_OPERATOR LESS_THAN_OPERATOR 
    GREATER_THAN_OR_EQUAL_OPERATOR LESS_THAN_OR_EQUAL_OPERATOR

/* Logical Operators */
%token <str>
    LOGICAL_AND_OPERATOR LOGICAL_OR_OPERATOR LOGICAL_NOT_OPERATOR

/* Bitwise Operators */   
%token <str>
    BITWISE_AND_OPERATOR BITWISE_OR_OPERATOR BITWISE_XOR_OPERATOR LEFT_SHIFT_OPERATOR 
    RIGHT_SHIFT_OPERATOR BITWISE_NOT_OPERATOR

/* Other Operators & Symbols */
%token <str>
    TERNARY_OPERATOR DOT_OPERATOR  SCOPE_RESOLUTION_OPERATOR  
    POINTER_TO_MEMBER_DOT_OPERATOR POINTER_TO_MEMBER_ARROW_OPERATOR

%nonassoc LOWER_THAN_ELSE
%nonassoc KEYWORD_ELSE

%type<node> translation_unit external_declaration function_definition

%type<node> declaration declaration_specifiers declaration_specifier declarator declaration_list compound_statement struct_declaration_list

%type<node> storage_class_specifier type_specifier struct_specifier class_specifier member_declaration_list member_declaration access_specifier

%type<node> struct_declaration struct_declarator_list struct_declarator specifier_qualifier_list type_qualifier constant_expression

%type<node> type_qualifier_list parameter_type_list parameter_list parameter_declaration identifier_list type_name abstract_declarator

%type<node> initializer initializer_list direct_declarator pointer direct_abstract_declarator assignment_expression

%type<node> statement labeled_statement statement_list expression_statement selection_statement iteration_statement jump_statement

%type<node> expression init_declarator init_declarator_list conditional_expression primary_expression postfix_expression

%type<node> unary_expression unary_operator cast_expression multiplicative_expression additive_expression shift_expression

%type<node> relational_expression equality_expression and_expression exclusive_or_expression inclusive_or_expression

%type<node> logical_and_expression logical_or_expression argument_expression_list assignment_operator try_catch_statement io_statement
%start translation_unit
%%

primary_expression
	: ID { $$ = $1; }
	| INTEGER { $$ = $1; }
    | FLOAT { $$ = $1; }
	| STRING { $$ = $1; }
	| CHAR { $$ = $1; }
	| BOOLEAN_LITERAL {$$ = $1; } 
    | KEYWORD_NULLPTR {$$ = $1; }
    | KEYWORD_THIS
	| LPAREN expression RPAREN { $$ = $2; }
	;

postfix_expression
	: primary_expression { $$ = $1; }
	| postfix_expression LBRACKET expression RBRACKET { $$ = createNode(NODE_POSTFIX_EXPRESSION, monostate(), $1, $3); }
	| postfix_expression LPAREN RPAREN { $$ = createNode(NODE_POSTFIX_EXPRESSION, monostate(), $1); }
	| postfix_expression LPAREN argument_expression_list RPAREN { $$ = createNode(NODE_POSTFIX_EXPRESSION, monostate(), $1, $3); }
	| postfix_expression DOT_OPERATOR ID { $$ = createNode(NODE_POSTFIX_EXPRESSION, monostate(), $1, $3); }
	| postfix_expression POINTER_TO_MEMBER_ARROW_OPERATOR ID { $$ = createNode(NODE_POSTFIX_EXPRESSION, monostate(), $1, $3); }
    | postfix_expression POINTER_TO_MEMBER_DOT_OPERATOR ID { $$ = createNode(NODE_POSTFIX_EXPRESSION, monostate(), $1, $3); }
	| postfix_expression INCREMENT_OPERATOR { $$ = createNode(NODE_POSTFIX_EXPRESSION, monostate(), $1); }
	| postfix_expression DECREMENT_OPERATOR { $$ = createNode(NODE_POSTFIX_EXPRESSION, monostate(), $1); }
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
	: postfix_expression { $$ = $1; }
	| INCREMENT_OPERATOR unary_expression { $$ = createNode(NODE_UNARY_EXPRESSION, monostate(), $2); }
	| DECREMENT_OPERATOR unary_expression { $$ = createNode(NODE_UNARY_EXPRESSION, monostate(), $2); }
	| unary_operator cast_expression { $$ = createNode(NODE_UNARY_EXPRESSION, monostate(), $1, $2); }
	| KEYWORD_SIZEOF unary_expression { $$ = createNode(NODE_UNARY_EXPRESSION, monostate(),$1, $2); }
	| KEYWORD_SIZEOF LPAREN type_name RPAREN { $$ = createNode(NODE_UNARY_EXPRESSION, monostate(), $1, $3);}
	| KEYWORD_NEW LPAREN type_name RPAREN { $$ = createNode(NODE_UNARY_EXPRESSION, monostate(), $3); }
	| KEYWORD_NEW LPAREN type_name RPAREN LBRACKET expression RBRACKET { $$ = createNode(NODE_UNARY_EXPRESSION, monostate(), $3, $6); }
	| KEYWORD_DELETE cast_expression
