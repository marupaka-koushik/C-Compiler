%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
%}

%token IDENTIFIER INTEGER
%token INT VOID SEMICOLON

%%

program:
    declaration
    ;

declaration:
    INT IDENTIFIER SEMICOLON
    ;

%%

int main() {
    return yyparse();
}
