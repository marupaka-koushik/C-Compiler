#!/bin/bash
make clean
make
if [ -f build/lexer ]; then
    echo "Build successful"
fi
