CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -g
LEX = flex
YACC = bison

SOURCES = src/parser.tab.c src/lex.yy.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = compiler

all: $(TARGET)

src/lex.yy.c: src/lexer.l
	$(LEX) -o $@ $<

src/parser.tab.c src/parser.tab.h: src/parser.y
	$(YACC) -d -o src/parser.tab.c $<

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.tab.* src/lex.yy.* $(OBJECTS) $(TARGET)
	rm -rf output/*.asm output/*.tac

.PHONY: all clean
