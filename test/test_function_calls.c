// Comprehensive function call with arguments testing

// Test 1: Function with single integer argument
int add5(int x) {
    return x + 5;
}

// Test 2: Function with two integer arguments
int add(int a, int b) {
    return a + b;
}

// Test 3: Function with three arguments
int sum3(int x, int y, int z) {
    return x + y + z;
}

// Test 4: Function with multiple operations
int multiply(int a, int b) {
    int result = a * b;
    return result;
}

// Test 5: Function calling another function
int addAndDouble(int x, int y) {
    int sum = add(x, y);
    return sum * 2;
}

// Test 6: Function with expression as argument
int square(int n) {
    return n * n;
}

// Test 7: Function with character argument
char getNext(char c) {
    return c + 1;
}

// Test 8: Function with mixed argument types
int compute(int a, char b) {
    return a + b;
}

// Test 9: Recursive function
int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

// Test 10: Function with array element as argument
int increment(int val) {
    return val + 1;
}

// Test 11: Function with pointer argument
int getValue(int *ptr) {
    return *ptr;
}

// Test 12: Function with multiple return paths
int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

// Test 13: Function with local variables
int subtract(int x, int y) {
    int temp = x - y;
    int result = temp;
    return result;
}

// Test 14: Nested function calls
int doubleValue(int n) {
    return n * 2;
}

int quadruple(int n) {
    return doubleValue(doubleValue(n));
}

int main() {
    // Test 1: Simple function call with literal
    int result1 = add5(10);
    
    // Test 2: Function call with two literals
    int result2 = add(5, 3);
    
    // Test 3: Function call with variables
    int x = 10;
    int y = 20;
    int result3 = add(x, y);
    
    // Test 4: Function call with three arguments
    int result4 = sum3(1, 2, 3);
    
    // Test 5: Function call with expressions
    int a = 5;
    int b = 3;
    int result5 = add(a + 1, b - 1);
    
    // Test 6: Nested function calls
    int result6 = add(add(2, 3), add(4, 5));
    
    // Test 7: Function call in expression
    int result7 = add5(10) + add5(20);
    
    // Test 8: Function call in condition
    if (add(2, 3) > 4) {
        int valid = 1;
    }
    
    // Test 9: Function call in loop condition
    int i;
    for (i = 0; i < add(3, 2); i = i + 1) {
        int temp = i;
    }
    
    // Test 10: Function call with array element
    int arr[3];
    arr[0] = 10;
    arr[1] = 20;
    int result8 = add(arr[0], arr[1]);
    
    // Test 11: Multiple function calls in sequence
    int r1 = add(1, 2);
    int r2 = add(3, 4);
    int r3 = add(r1, r2);
    
    // Test 12: Function returning value used in another call
    int result9 = addAndDouble(5, 10);
    
    // Test 13: Function call with square
    int result10 = square(5);
    
    // Test 14: Function call with character
    char ch = 'A';
    char nextCh = getNext(ch);
    
    // Test 15: Function with mixed types
    int result11 = compute(10, 'B');
    
    // Test 16: Recursive function call
    int result12 = factorial(5);
    
    // Test 17: Function call in while loop
    int count = 5;
    while (count > 0) {
        int val = add5(count);
        count = count - 1;
    }
    
    // Test 18: Function call with pointer
    int num = 42;
    int *ptr = &num;
    int result13 = getValue(ptr);
    
    // Test 19: Function with max
    int result14 = max(15, 25);
    
    // Test 20: Function with complex expression
    int m = 10;
    int n = 5;
    int result15 = add(m * 2, n / 2);
    
    // Test 21: Quadruple nested calls
    int result16 = quadruple(3);
    
    // Test 22: Function call result in assignment chain
    int p = add(1, 2);
    int q = p + add(3, 4);
    
    // Test 23: Function in arithmetic expression
    int result17 = multiply(4, 5) + multiply(2, 3);
    
    // Test 24: Function call in nested loop
    int j;
    int k;
    for (j = 0; j < 2; j = j + 1) {
        for (k = 0; k < 2; k = k + 1) {
            int val = add(j, k);
        }
    }
    
    // Test 25: Function call with all operations
    int base = 10;
    int result18 = add(square(base), multiply(base, 2));
    
    return 0;
}
