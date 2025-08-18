#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct symbol {
    char *name;
    char *type;
    int scope;
    struct symbol *next;
} Symbol;

Symbol* createSymbol(char *name, char *type, int scope);
void insertSymbol(Symbol **table, Symbol *sym);
Symbol* lookupSymbol(Symbol *table, char *name);

#endif
