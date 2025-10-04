// Comprehensive test for function calls with arguments - VALID cases

// Function definitions (no forward declarations to avoid symbol table issues)
int add(int a, int b) {
    return a + b;
}

int multiply(int x, int y) {
    return x * y;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

void printNumber(int n) {
    printf("%d\n", n);
}

int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int power(int base, int exp) {
    int result = 1;
    int i;
    for (i = 0; i < exp; i = i + 1) {
        result = result * base;
    }
    return result;
}

int main() {
    // 1. Basic function call with literal arguments
    int sum1 = add(5, 3);
    
    // 2. Function call with variable arguments
    int x = 10;
    int y = 20;
    int sum2 = add(x, y);
    
    // 3. Nested function calls
    int result1 = add(multiply(2, 3), multiply(4, 5));
    
    // 4. Function call as argument
    int result2 = multiply(add(2, 3), add(4, 5));
    
    // 5. Void function call
    printNumber(42);
    
    // 6. Function call in expression
    int expr1 = add(10, 20) + 5;
    int expr2 = multiply(3, 4) * 2;
    
    // 7. Function call in conditional
    if (max(x, y) > 15) {
        int temp = 1;
    }
    
    // 8. Function call in loop condition
    int i;
    for (i = 0; i < max(5, 10); i = i + 1) {
        int dummy = i;
    }
    
    // 9. Function call in while loop
    int count = 0;
    while (count < max(3, 7)) {
        count = count + 1;
    }
    
    // 10. Recursive function call
    int fact5 = factorial(5);
    int fib7 = fibonacci(7);
    
    // 11. Function call with array elements
    int arr[3];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    int sum_arr = add(arr[0], arr[1]);
    
    // 12. Function call with expressions as arguments
    int a = 5;
    int b = 3;
    int result3 = add(a + b, a - b);
    int result4 = multiply(a * 2, b + 1);
    
    // 13. Function call in switch
    int choice = max(2, 4);
    int switch_val = 0;
    switch (choice) {
        case 4:
            switch_val = add(1, 2);
            break;
        default:
            switch_val = 0;
            break;
    }
    
    // 14. Multiple function calls in one statement
    int result5 = add(10, 20) + multiply(3, 4) - max(5, 8);
    
    // 15. Function call with struct member
    struct Point {
        int x;
        int y;
    };
    struct Point p;
    p.x = 10;
    p.y = 20;
    int sum_point = add(p.x, p.y);
    
    // 16. Function call assigned to array element
    int results[5];
    results[0] = add(1, 2);
    results[1] = multiply(3, 4);
    results[2] = max(10, 15);
    
    // 17. Function call in ternary operator
    int max_val = (x > y) ? max(x, 100) : max(y, 100);
    
    // 18. Chain of function calls
    int chain = add(add(1, 2), add(3, 4));
    
    // 19. Function call with bitwise operations
    int bit_result = add(x & y, x | y);
    
    // 20. Function call with logical expressions
    int logic = max(x && y, x || y);
    
    // 21. GCD calculation
    int gcd_result = gcd(48, 18);
    
    // 22. Power calculation
    int pow_result = power(2, 10);
    
    // 23. Function calls in printf
    printf("Sum: %d\n", add(10, 20));
    printf("Product: %d\n", multiply(5, 6));
    
    // 24. Function call result used in comparison
    if (add(5, 5) == 10) {
        int correct = 1;
    }
    
    // 25. Complex nested calls
    int complex = add(
        multiply(add(1, 2), add(3, 4)),
        multiply(add(5, 6), add(7, 8))
    );
    
    return 0;
}
