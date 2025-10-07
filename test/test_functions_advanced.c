// Advanced function call tests

// Test with pointer argument
int getValue(int *ptr) {
    return *ptr;
}

// Test with character
char getNext(char c) {
    return c + 1;
}

// Test recursive function
int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

// Test function calling another function
int addAndDouble(int x, int y) {
    int sum = x + y;
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
    
    // Test 3: Recursive call
    int r3 = factorial(5);
    
    // Test 4: Function calling function
    int r4 = addAndDouble(5, 10);
    
    // Test 5: Nested call with same function
    int r5 = doubleValue(doubleValue(3));
    
    return 0;
}
