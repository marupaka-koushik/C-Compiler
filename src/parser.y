%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
void yyerror(const char* s);
extern int line_num;
%}

%union {
    int ival;
    float fval;
    char* str;
    void* node;
}

%token <str> IDENTIFIER STRING_LITERAL
%token <ival> INTEGER_LITERAL CHAR_LITERAL
%token <fval> FLOAT_LITERAL

%token INT CHAR VOID FLOAT DOUBLE LONG SHORT UNSIGNED SIGNED
%token IF ELSE WHILE FOR DO RETURN BREAK CONTINUE
%token STRUCT UNION ENUM TYPEDEF
%token PLUS MINUS STAR DIVIDE MOD
%token INC DEC
%token ASSIGN PLUS_ASSIGN MINUS_ASSIGN STAR_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token EQ NE LT GT LE GE
%token AND OR NOT
%token AMPERSAND PIPE XOR TILDE LEFT_SHIFT RIGHT_SHIFT
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
%token SEMICOLON COMMA DOT ARROW

%type <node> program translation_unit external_declaration
%type <node> function_definition declaration
%type <node> declaration_specifiers type_specifier
%type <node> declarator direct_declarator
%type <node> parameter_list parameter_declaration
%type <node> compound_statement statement_list statement
%type <node> expression_statement selection_statement iteration_statement
%type <node> jump_statement expression assignment_expression
%type <node> conditional_expression logical_or_expression logical_and_expression
%type <node> equality_expression relational_expression
%type <node> additive_expression multiplicative_expression
%type <node> unary_expression postfix_expression primary_expression
%type <node> argument_expression_list

%right ASSIGN PLUS_ASSIGN MINUS_ASSIGN STAR_ASSIGN DIV_ASSIGN MOD_ASSIGN
%left OR
%left AND
%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left STAR DIVIDE MOD
%right NOT UMINUS
%left LPAREN RPAREN LBRACKET RBRACKET DOT ARROW

%start program

%%

program
    : translation_unit
        { printf("Parsing completed successfully\n"); }
    ;

translation_unit
    : external_declaration
    | translation_unit external_declaration
    ;

external_declaration
    : function_definition
    | declaration
    ;

function_definition
    : declaration_specifiers declarator compound_statement
        { printf("Function definition parsed\n"); }
    ;

declaration
    : declaration_specifiers declarator SEMICOLON
        { printf("Declaration parsed\n"); }
    | declaration_specifiers declarator ASSIGN assignment_expression SEMICOLON
        { printf("Declaration with initialization parsed\n"); }
    ;

declaration_specifiers
    : type_specifier
    ;

type_specifier
    : INT       { printf("Type: int\n"); }
    | CHAR      { printf("Type: char\n"); }
    | VOID      { printf("Type: void\n"); }
    | FLOAT     { printf("Type: float\n"); }
    | DOUBLE    { printf("Type: double\n"); }
    ;

declarator
    : direct_declarator
    | STAR direct_declarator
    ;

direct_declarator
    : IDENTIFIER
        { printf("Declarator: %s\n", $1); }
    | direct_declarator LPAREN parameter_list RPAREN
    | direct_declarator LPAREN RPAREN
    | direct_declarator LBRACKET INTEGER_LITERAL RBRACKET
    | direct_declarator LBRACKET RBRACKET
    | LPAREN declarator RPAREN
    ;

parameter_list
    : parameter_declaration
    | parameter_list COMMA parameter_declaration
    ;

parameter_declaration
    : declaration_specifiers declarator
    | declaration_specifiers
    ;

compound_statement
    : LBRACE RBRACE
    | LBRACE statement_list RBRACE
    ;

statement_list
    : statement
    | statement_list statement
    ;

statement
    : compound_statement
    | expression_statement
    | selection_statement
    | iteration_statement
    | jump_statement
    ;

expression_statement
    : SEMICOLON
    | expression SEMICOLON
    ;

expression
    : assignment_expression
    | expression COMMA assignment_expression
    ;

assignment_expression
    : conditional_expression
    | unary_expression ASSIGN assignment_expression
    | unary_expression PLUS_ASSIGN assignment_expression
    | unary_expression MINUS_ASSIGN assignment_expression
    | unary_expression STAR_ASSIGN assignment_expression
    | unary_expression DIV_ASSIGN assignment_expression
    | unary_expression MOD_ASSIGN assignment_expression
    ;

conditional_expression
    : logical_or_expression
    ;

logical_or_expression
    : logical_and_expression
    | logical_or_expression OR logical_and_expression
        { printf("Logical OR expression\n"); }
    ;

logical_and_expression
    : equality_expression
    | logical_and_expression AND equality_expression
        { printf("Logical AND expression\n"); }
    ;

equality_expression
    : relational_expression
    | equality_expression EQ relational_expression
        { printf("Equality expression\n"); }
    | equality_expression NE relational_expression
        { printf("Not-equal expression\n"); }
    ;

relational_expression
    : additive_expression
    | relational_expression LT additive_expression
        { printf("Less-than expression\n"); }
    | relational_expression GT additive_expression
        { printf("Greater-than expression\n"); }
    | relational_expression LE additive_expression
        { printf("Less-equal expression\n"); }
    | relational_expression GE additive_expression
        { printf("Greater-equal expression\n"); }
    ;

additive_expression
    : multiplicative_expression
    | additive_expression PLUS multiplicative_expression
        { printf("Addition expression\n"); }
    | additive_expression MINUS multiplicative_expression
        { printf("Subtraction expression\n"); }
    ;

multiplicative_expression
    : unary_expression
    | multiplicative_expression STAR unary_expression
        { printf("Multiplication expression\n"); }
    | multiplicative_expression DIVIDE unary_expression
        { printf("Division expression\n"); }
    | multiplicative_expression MOD unary_expression
        { printf("Modulo expression\n"); }
    ;

unary_expression
    : postfix_expression
    | INC unary_expression
        { printf("Pre-increment\n"); }
    | DEC unary_expression
        { printf("Pre-decrement\n"); }
    | PLUS unary_expression
    | MINUS unary_expression %prec UMINUS
        { printf("Unary minus\n"); }
    | NOT unary_expression
        { printf("Logical NOT\n"); }
    | STAR unary_expression
        { printf("Dereference\n"); }
    | AMPERSAND unary_expression
        { printf("Address-of\n"); }
    ;

postfix_expression
    : primary_expression
    | postfix_expression LBRACKET expression RBRACKET
        { printf("Array subscript\n"); }
    | postfix_expression LPAREN RPAREN
        { printf("Function call (no args)\n"); }
    | postfix_expression LPAREN argument_expression_list RPAREN
        { printf("Function call with args\n"); }
    | postfix_expression DOT IDENTIFIER
        { printf("Member access: .%s\n", $3); }
    | postfix_expression ARROW IDENTIFIER
        { printf("Pointer member access: ->%s\n", $3); }
    | postfix_expression INC
        { printf("Post-increment\n"); }
    | postfix_expression DEC
        { printf("Post-decrement\n"); }
    ;

argument_expression_list
    : assignment_expression
    | argument_expression_list COMMA assignment_expression
    ;

primary_expression
    : IDENTIFIER            { printf("Identifier: %s\n", $1); }
    | INTEGER_LITERAL       { printf("Integer: %d\n", $1); }
    | CHAR_LITERAL          { printf("Char: %c\n", $1); }
    | FLOAT_LITERAL         { printf("Float: %f\n", $1); }
    | STRING_LITERAL        { printf("String: %s\n", $1); }
    | LPAREN expression RPAREN
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Parse error at line %d: %s\n", line_num, s);
}

int main(int argc, char** argv) {
    if (argc > 1) {
        extern FILE* yyin;
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            fprintf(stderr, "Cannot open file: %s\n", argv[1]);
            return 1;
        }
    }
    
    return yyparse();
}
