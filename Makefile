LEX = flex
CC = gcc
CFLAGS = -Wall

all: lexer

lexer: src/lexer.l
	$(LEX) -o build/lex.yy.c src/lexer.l
	$(CC) $(CFLAGS) build/lex.yy.c -o build/lexer -lfl

clean:
	rm -rf build/*

.PHONY: all clean
