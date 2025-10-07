// Advanced function call tests (no recursion)

// Test with pointer argument
int getValue(int *ptr) {
    return *ptr;
}

// Test with character
char getNext(char c) {
    return c + 1;
}

// Test function calling another function
int add(int x, int y) {
    return x + y;
}

int addAndDouble(int x, int y) {
    int sum = add(x, y);
    return sum * 2;
}

int doubleValue(int n) {
    return n * 2;
}

int main() {
    // Test 1: Pointer argument
    int num = 42;
    int *ptr = &num;
    int r1 = getValue(ptr);
    
    // Test 2: Character argument
    char ch = 'A';
    char r2 = getNext(ch);
    
    // Test 3: Function calling function
    int r3 = addAndDouble(5, 10);
    
    // Test 4: Nested call with same function
    int r4 = doubleValue(doubleValue(3));
    
    // Test 5: Multiple levels
    int r5 = add(add(1, 2), add(3, 4));
    
    return 0;
}
