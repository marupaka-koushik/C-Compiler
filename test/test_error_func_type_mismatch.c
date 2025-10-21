// Error: Type mismatch in function arguments

int add(int a, int b) {
    return a + b;
}

int main() {
    char c = 'A';
    
    // ERROR: add expects int arguments, but char provided
    // This may or may not be caught depending on type checking strictness
    int result = add(c, c);
    
    return 0;
}
