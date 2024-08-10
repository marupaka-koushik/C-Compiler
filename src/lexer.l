%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
"long"              { return LONG; }
"short"             { return SHORT; }
"unsigned"          { return UNSIGNED; }
"signed"            { return SIGNED; }

"if"                { return IF; }
"else"              { return ELSE; }
"while"             { return WHILE; }
"for"               { return FOR; }
"do"                { return DO; }
"return"            { return RETURN; }
"break"             { return BREAK; }
"continue"          { return CONTINUE; }

"struct"            { return STRUCT; }
"union"             { return UNION; }
"enum"              { return ENUM; }

"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return STAR; }
"/"                 { return DIVIDE; }
"%"                 { return MOD; }

"++"                { return INC; }
"--"                { return DEC; }

"="                 { return ASSIGN; }
"+="                { return PLUS_ASSIGN; }
"-="                { return MINUS_ASSIGN; }
"*="                { return STAR_ASSIGN; }
"/="                { return DIV_ASSIGN; }
"%="                { return MOD_ASSIGN; }

"=="                { return EQ; }
"!="                { return NE; }
"<"                 { return LT; }
">"                 { return GT; }
"<="                { return LE; }
">="                { return GE; }

"&&"                { return AND; }
"||"                { return OR; }
"!"                 { return NOT; }

"&"                 { return AMPERSAND; }
"|"                 { return PIPE; }
"^"                 { return XOR; }
"~"                 { return TILDE; }
"<<"                { return LEFT_SHIFT; }
">>"                { return RIGHT_SHIFT; }

"("                 { return LPAREN; }
")"                 { return RPAREN; }
"{"                 { return LBRACE; }
"}"                 { return RBRACE; }
"["                 { return LBRACKET; }
"]"                 { return RBRACKET; }

";"                 { return SEMICOLON; }
","                 { return COMMA; }
"."                 { return DOT; }
"->"                { return ARROW; }

{LETTER}{ALNUM}*    { 
                        yylval.str = strdup(yytext); 
                        return IDENTIFIER; 
                    }

{DIGIT}+            { 
                        yylval.ival = atoi(yytext); 
                        return INTEGER_LITERAL; 
                    }

0[xX]{HEX}+         { 
                        yylval.ival = strtol(yytext, NULL, 16); 
                        return INTEGER_LITERAL; 
                    }

0{OCTAL}+           { 
                        yylval.ival = strtol(yytext, NULL, 8); 
                        return INTEGER_LITERAL; 
                    }

{DIGIT}+\.{DIGIT}+  { 
                        yylval.fval = atof(yytext); 
                        return FLOAT_LITERAL; 
                    }

\"([^\\\"]|\\.)*\"  { 
                        yylval.str = strdup(yytext); 
                        return STRING_LITERAL; 
                    }

'([^\\']|\\.)'      { 
                        yylval.ival = yytext[1]; 
                        return CHAR_LITERAL; 
                    }

"//".* { /* Single line comment */ }

"/*"                { 
                        int c;
                        while ((c = yyinput()) != 0) {
                            if (c == '\n') {
                                line_num++;
                                col_num = 1;
                            } else if (c == '*') {
                                if ((c = yyinput()) == '/') break;
                                unput(c);
                            }
                        }
                    }

.                   { 
                        fprintf(stderr, "Lexical error: Unknown character '%s' at line %d, col %d\n", 
                                yytext, line_num, col_num); 
                    }

%%

void lexer_error(const char* msg) {
    fprintf(stderr, "Lexer Error at line %d, col %d: %s\n", line_num, col_num, msg);
}

int get_line_number() {
    return line_num;
}

int get_column_number() {
    return col_num;
}

void reset_lexer() {
    line_num = 1;
    col_num = 1;
}
