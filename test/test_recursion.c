// Test recursive function calls

// Test 1: Simple recursion - factorial
int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

// Test 2: Fibonacci with multiple recursive calls
int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Test 3: Recursive sum
int sum(int n) {
    if (n == 0) {
        return 0;
    }
    return n + sum(n - 1);
}

// Test 4: Mutual recursion - even/odd
int isEven(int n);
int isOdd(int n);

int isEven(int n) {
    if (n == 0) {
        return 1;
    }
    return isOdd(n - 1);
}

int isOdd(int n) {
    if (n == 0) {
        return 0;
    }
    return isEven(n - 1);
}

// Test 5: Recursion with multiple parameters
int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

// Test 6: Tail recursion
int tailFactorial(int n, int acc) {
    if (n <= 1) {
        return acc;
    }
    return tailFactorial(n - 1, n * acc);
}

// Test 7: Nested recursive calls
int ackermann(int m, int n) {
    if (m == 0) {
        return n + 1;
    }
    if (n == 0) {
        return ackermann(m - 1, 1);
    }
    return ackermann(m - 1, ackermann(m, n - 1));
}

int main() {
    // Test factorial
    int f5 = factorial(5);  // Should be 120
    
    // Test fibonacci
    int fib6 = fibonacci(6);  // Should be 8
    
    // Test sum
    int s10 = sum(10);  // Should be 55
    
    // Test even/odd
    int e4 = isEven(4);  // Should be 1
    int o5 = isOdd(5);   // Should be 1
    
    // Test gcd
    int g = gcd(48, 18);  // Should be 6
    
    // Test tail recursion
    int tf5 = tailFactorial(5, 1);  // Should be 120
    
    // Test ackermann (small values only!)
    int a = ackermann(2, 2);  // Should be 7
    
    return 0;
}
