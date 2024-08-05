# C Compiler Project

A complete C compiler implementation with lexical analysis, parsing, 
intermediate representation generation, and MIPS code generation.

## Features
- Full C language support
- Lexical analysis with Flex
- Syntax parsing with Bison
- Three-address code generation
- MIPS assembly code generation
- Comprehensive test suite

## Build
```bash
make clean
make
```

## Run
```bash
./run.sh tests/1.c
```

## Architecture
1. Lexer (src/lexer.l) - Tokenization
2. Parser (src/parser.y) - Syntax analysis and IR generation
3. Symbol Table (inc/symbolTable.h) - Identifier management
4. AST (inc/treeNode.h) - Abstract syntax tree
5. Code Generator (inc/mipsGenerator.h) - MIPS output

## Authors
- marupaka-koushik
- ravitejajaligama
- Togarucharitha
- MelliCorleone1972

## License
MIT License
