%{
#include <stdio.h>
#include <stdlib.h>
%}

%%

[ \t\n]+    { /* skip whitespace */ }
.           { printf("CHAR: %s\n", yytext); }

%%

int yywrap() { return 1; }

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (file) {
            yyin = file;
        }
    }
    yylex();
    return 0;
}
