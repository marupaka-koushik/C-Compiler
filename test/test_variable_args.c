// Test printf with different number of arguments (not true varargs)

int main() {
    // Test 1: printf with 1 argument (string only)
    printf("Hello\n");
    
    // Test 2: printf with 2 arguments (string + int)
    int x = 42;
    printf("Value: %d\n", x);
    
    // Test 3: printf with 3 arguments (string + 2 ints)
    int a = 10;
    int b = 20;
    printf("a=%d, b=%d\n", a, b);
    
    // Test 4: printf with 4 arguments (string + 3 ints)
    int i = 1;
    int j = 2;
    int k = 3;
    printf("i=%d, j=%d, k=%d\n", i, j, k);
    
    // Test 5: printf with 5 arguments
    printf("%d %d %d %d\n", 1, 2, 3, 4);
    
    // Test 6: scanf with different arguments
    int n1;
    scanf("%d", &n1);
    
    int n2;
    int n3;
    scanf("%d %d", &n2, &n3);
    
    // Test 7: Function calls with different arg counts
    int r1 = add(5, 3);
    int r2 = sum3(1, 2, 3);
    
    return 0;
}

// Helper functions
int add(int a, int b) {
    return a + b;
}

int sum3(int a, int b, int c) {
    return a + b + c;
}
