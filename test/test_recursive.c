// Comprehensive recursive function call testing

// Test 1: Factorial (simple recursion)
int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

// Test 2: Fibonacci (double recursion)
int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Test 3: Sum from 1 to n (tail recursion)
int sum(int n) {
    if (n == 0) {
        return 0;
    }
    return n + sum(n - 1);
}

// Test 4: Power function (recursive)
int power(int base, int exp) {
    if (exp == 0) {
        return 1;
    }
    return base * power(base, exp - 1);
}

// Test 5: Count down (simple recursion)
int countdown(int n) {
    if (n <= 0) {
        return 0;
    }
    return countdown(n - 1);
}

// Test 6: GCD (Euclidean algorithm - mutual recursion style)
int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

// Test 7: Recursive with multiple base cases
int multiBase(int n) {
    if (n <= 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    if (n == 2) {
        return 2;
    }
    return multiBase(n - 1) + multiBase(n - 2);
}

// Test 8: Array sum recursive
int arraySum(int arr[], int size) {
    if (size == 0) {
        return 0;
    }
    return arr[size - 1] + arraySum(arr, size - 1);
}

int main() {
    // Test 1: Factorial
    int fact5 = factorial(5);
    int fact3 = factorial(3);
    
    // Test 2: Fibonacci
    int fib6 = fibonacci(6);
    int fib4 = fibonacci(4);
    
    // Test 3: Sum
    int sum10 = sum(10);
    int sum5 = sum(5);
    
    // Test 4: Power
    int pow2_3 = power(2, 3);
    int pow3_2 = power(3, 2);
    
    // Test 5: Countdown
    int cd = countdown(5);
    
    // Test 6: GCD
    int gcd1 = gcd(48, 18);
    int gcd2 = gcd(100, 50);
    
    // Test 7: Multiple base cases
    int mb = multiBase(5);
    
    // Test 8: Array sum
    int arr[5];
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    arr[3] = 4;
    arr[4] = 5;
    int total = arraySum(arr, 5);
    
    // Test 9: Recursive in loop
    int i;
    for (i = 1; i <= 3; i = i + 1) {
        int f = factorial(i);
    }
    
    // Test 10: Recursive in condition
    if (factorial(3) > 5) {
        int check = 1;
    }
    
    // Test 11: Nested recursive calls
    int nested = factorial(fibonacci(4));
    
    // Test 12: Recursive result in expression
    int expr = factorial(3) + factorial(4);
    
    // Test 13: Multiple recursive calls in same expression
    int multi = fibonacci(3) + fibonacci(4) + fibonacci(5);
    
    // Test 14: Recursive call as parameter
    int param = power(factorial(3), 2);
    
    // Test 15: Recursive in nested loop
    int j;
    int k;
    for (j = 1; j <= 2; j = j + 1) {
        for (k = 1; k <= 2; k = k + 1) {
            int temp = sum(j + k);
        }
    }
    
    return 0;
}
