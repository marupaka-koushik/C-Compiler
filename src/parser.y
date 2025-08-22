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
%token PLUS MINUS STAR SLASH ASSIGN
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
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
    type_specifier declarator SEMICOLON
    | type_specifier declarator compound_stmt
    ;

type_specifier:
    INT | CHAR | VOID
    ;

declarator:
    IDENTIFIER
    | STAR declarator
    | declarator LBRACKET INTEGER RBRACKET
    | declarator LPAREN RPAREN
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
    | expression ASSIGN expression
    | STAR expression
    | expression LBRACKET expression RBRACKET
    | LPAREN expression RPAREN
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}

int main() {
    return yyparse();
}

/* Struct declarations will be added */
