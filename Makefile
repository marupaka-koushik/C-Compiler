LEX = flex
YACC = bison
CC = gcc
CFLAGS = -Wall

all: parser

parser: src/parser.y src/lexer.l
	$(YACC) -d -o build/y.tab.c src/parser.y
	$(LEX) -o build/lex.yy.c src/lexer.l
	$(CC) $(CFLAGS) build/y.tab.c build/lex.yy.c -o build/parser -lfl

clean:
	rm -rf build/*

.PHONY: all clean
