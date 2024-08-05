#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 <input.c>"
    exit 1
fi

INPUT=$1
BASENAME=$(basename "$INPUT" .c)

echo "Compiling $INPUT..."
./compiler "$INPUT"

if [ -f "output/${BASENAME}.asm" ]; then
    echo "✓ Assembly generated: output/${BASENAME}.asm"
    echo "✓ TAC generated: output/${BASENAME}.tac"
else
    echo "✗ Compilation failed"
    exit 1
fi
