%{
#include <stdio.h>
#include <string.h>
#include "parser.tab.h"

int line_num = 1;
int col_num = 1;

#define YY_USER_ACTION \
    col_num += yyleng;

void yyerror(const char* s);
%}

%option noyywrap

DIGIT       [0-9]
LETTER      [a-zA-Z_]
ALNUM       [a-zA-Z0-9_]
HEX         [0-9a-fA-F]
OCTAL       [0-7]

%%

[ \t]+              { /* Skip whitespace */ }
\n                  { line_num++; col_num = 1; }

"int"               { return INT; }
"char"              { return CHAR; }
"void"              { return VOID; }
"float"             { return FLOAT; }
"double"            { return DOUBLE; }

"if"                { return IF; }
"else"              { return ELSE; }
"while"             { return WHILE; }
"for"               { return FOR; }
"return"            { return RETURN; }

"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return STAR; }
"/"                 { return DIVIDE; }
"%"                 { return MOD; }

"="                 { return ASSIGN; }
"=="                { return EQ; }
"!="                { return NE; }
"<"                 { return LT; }
">"                 { return GT; }
"<="                { return LE; }
">="                { return GE; }

"&&"                { return AND; }
"||"                { return OR; }
"!"                 { return NOT; }

"("                 { return LPAREN; }
")"                 { return RPAREN; }
"{"                 { return LBRACE; }
"}"                 { return RBRACE; }
"["                 { return LBRACKET; }
"]"                 { return RBRACKET; }

";"                 { return SEMICOLON; }
","                 { return COMMA; }

{LETTER}{ALNUM}*    { 
                        yylval.str = strdup(yytext); 
                        return IDENTIFIER; 
                    }

{DIGIT}+            { 
                        yylval.ival = atoi(yytext); 
                        return INTEGER_LITERAL; 
                    }

.                   { 
                        fprintf(stderr, "Unknown character: %s at line %d\n", 
                                yytext, line_num); 
                    }

%%
