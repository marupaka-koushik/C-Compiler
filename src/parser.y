%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%token IDENTIFIER INTEGER
%token INT CHAR VOID
%token SEMICOLON COMMA
%token LPAREN RPAREN

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
    | type_specifier IDENTIFIER LPAREN RPAREN SEMICOLON
    ;

type_specifier:
    INT | CHAR | VOID
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}

int main() {
    return yyparse();
}
