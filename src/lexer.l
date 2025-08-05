%{
#include <stdio.h>
#include <stdlib.h>
%}

%%

"int"       { printf("KEYWORD: %s\n", yytext); }
"char"      { printf("KEYWORD: %s\n", yytext); }
"void"      { printf("KEYWORD: %s\n", yytext); }
"if"        { printf("KEYWORD: %s\n", yytext); }
"else"      { printf("KEYWORD: %s\n", yytext); }
"while"     { printf("KEYWORD: %s\n", yytext); }
"for"       { printf("KEYWORD: %s\n", yytext); }
"return"    { printf("KEYWORD: %s\n", yytext); }

[a-zA-Z_][a-zA-Z0-9_]*  { printf("IDENTIFIER: %s\n", yytext); }
[0-9]+      { printf("INTEGER: %s\n", yytext); }

[ \t\n]+    { /* skip whitespace */ }
.           { printf("UNKNOWN: %s\n", yytext); }

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
