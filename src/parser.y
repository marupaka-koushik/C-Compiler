%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%token IDENTIFIER INTEGER
%token INT CHAR VOID
%token IF ELSE WHILE RETURN
%token SEMICOLON COMMA
%token PLUS MINUS STAR SLASH
%token LPAREN RPAREN LBRACE RBRACE

%%

program:
    declaration_list
    ;

declaration_list:
    declaration
    | declaration_list declaration
    ;

declaration:
    type_specifier IDENTIFIER SEMICOLON
    | type_specifier IDENTIFIER LPAREN RPAREN compound_stmt
    ;

type_specifier:
    INT | CHAR | VOID
    ;

compound_stmt:
    LBRACE statement_list RBRACE
    ;

statement_list:
    statement
    | statement_list statement
    ;

statement:
    expression SEMICOLON
    | RETURN expression SEMICOLON
    ;

expression:
    IDENTIFIER
    | INTEGER
    | expression PLUS expression
    | expression MINUS expression
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}

int main() {
    return yyparse();
}
