// Multiple simple recursive functions

// Factorial
int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

// Sum
int sum(int n) {
    if (n == 0) {
        return 0;
    }
    return n + sum(n - 1);
}

// Power
int power(int base, int exp) {
    if (exp == 0) {
        return 1;
    }
    return base * power(base, exp - 1);
}

int main() {
    // Test factorial
    int f1 = factorial(5);
    int f2 = factorial(3);
    
    // Test sum
    int s1 = sum(10);
    int s2 = sum(5);
    
    // Test power
    int p1 = power(2, 3);
    int p2 = power(3, 2);
    
    // Test in expression
    int expr = factorial(3) + sum(4);
    
    // Test in condition
    if (factorial(3) > 5) {
        int check = 1;
    }
    
    return 0;
}
