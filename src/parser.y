%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%token IDENTIFIER INTEGER
%token INT CHAR VOID
%token IF ELSE WHILE FOR RETURN
%token SEMICOLON COMMA
%token PLUS MINUS STAR SLASH
%token LPAREN RPAREN LBRACE RBRACE
%token EQ NE LT GT LE GE

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
    /* empty */
    | statement_list statement
    ;

statement:
    expression SEMICOLON
    | IF LPAREN expression RPAREN statement
    | IF LPAREN expression RPAREN statement ELSE statement
    | WHILE LPAREN expression RPAREN statement
    | RETURN expression SEMICOLON
    | compound_stmt
    ;

expression:
    IDENTIFIER
    | INTEGER
    | expression PLUS expression
    | expression MINUS expression
    | expression STAR expression
    | expression SLASH expression
    | expression EQ expression
    | expression NE expression
    | expression LT expression
    | expression GT expression
    | LPAREN expression RPAREN
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}

int main() {
    return yyparse();
}
