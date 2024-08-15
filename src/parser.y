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
    ;

direct_declarator
    : IDENTIFIER
        { printf("Declarator: %s\n", $1); }
    | direct_declarator LPAREN parameter_list RPAREN
    | direct_declarator LPAREN RPAREN
    | direct_declarator LBRACKET INTEGER_LITERAL RBRACKET
    | direct_declarator LBRACKET RBRACKET
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
