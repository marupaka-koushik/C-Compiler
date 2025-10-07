// Progressive function call tests

// Basic functions
int add(int a, int b) {
    return a + b;
}

int add5(int x) {
    return x + 5;
}

int sum3(int x, int y, int z) {
    return x + y + z;
}

int multiply(int a, int b) {
    return a * b;
}

int square(int n) {
    return n * n;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int main() {
    // Test 1: Simple call with literals
    int r1 = add(5, 3);
    
    // Test 2: Call with variables
    int x = 10;
    int y = 20;
    int r2 = add(x, y);
    
    // Test 3: Three arguments
    int r3 = sum3(1, 2, 3);
    
    // Test 4: Call with expressions
    int a = 5;
    int b = 3;
    int r4 = add(a + 1, b - 1);
    
    // Test 5: Nested function calls
    int r5 = add(add(2, 3), add(4, 5));
    
    // Test 6: Function call in expression
    int r6 = add5(10) + add5(20);
    
    // Test 7: Array element as argument
    int arr[2];
    arr[0] = 10;
    arr[1] = 20;
    int r7 = add(arr[0], arr[1]);
    
    // Test 8: Sequential calls
    int s1 = add(1, 2);
    int s2 = add(3, 4);
    int s3 = add(s1, s2);
    
    // Test 9: Function in condition
    if (add(2, 3) > 4) {
        int valid = 1;
    }
    
    // Test 10: Function in loop
    int i;
    for (i = 0; i < add(3, 2); i = i + 1) {
        int temp = i;
    }
    
    // Test 11: Square function
    int r8 = square(5);
    
    // Test 12: Max function with if-else
    int r9 = max(15, 25);
    
    // Test 13: Multiple operations
    int r10 = multiply(4, 5) + multiply(2, 3);
    
    // Test 14: Function in while loop
    int count = 5;
    while (count > 0) {
        int val = add5(count);
        count = count - 1;
    }
    
    // Test 15: Complex expression
    int m = 10;
    int n = 5;
    int r11 = add(m * 2, n / 2);
    
    return 0;
}
