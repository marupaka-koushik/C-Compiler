// Comprehensive test for recursive function calls
// Tests: direct recursion, indirect recursion, tail recursion, multiple recursion

// Test 1: Simple factorial (direct recursion)
int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

// Test 2: Fibonacci (direct recursion with multiple calls)
int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Test 3: Tail recursive factorial
int factorial_tail_helper(int n, int acc) {
    if (n <= 1) {
        return acc;
    }
    return factorial_tail_helper(n - 1, n * acc);
}

int factorial_tail(int n) {
    return factorial_tail_helper(n, 1);
}

// Test 4: Sum of digits (recursion with division)
int sum_digits(int n) {
    if (n == 0) {
        return 0;
    }
    return (n % 10) + sum_digits(n / 10);
}

// Test 5: Power function (recursion with exponentiation)
int power(int base, int exp) {
    if (exp == 0) {
        return 1;
    }
    if (exp == 1) {
        return base;
    }
    return base * power(base, exp - 1);
}

// Test 6: GCD using Euclidean algorithm (recursion)
int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

// Test 7: Count down (simple recursion)
int countdown(int n) {
    if (n <= 0) {
        return 0;
    }
    return 1 + countdown(n - 1);
}

// Test 8: Array sum with recursion
int array_sum(int arr[], int size) {
    if (size <= 0) {
        return 0;
    }
    return arr[size - 1] + array_sum(arr, size - 1);
}

// Test 9: String length recursion (using pointer)
int str_length(char *s) {
    if (*s == '\0') {
        return 0;
    }
    return 1 + str_length(s + 1);
}

// Test 10: Indirect recursion - even/odd check
int is_even(int n) {
    if (n == 0) {
        return 1;
    }
    if (n == 1) {
        return 0;
    }
    return is_even(n - 2);
}

int is_odd(int n) {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    return is_odd(n - 2);
}

// Test 11: Ackermann function (double recursion)
int ackermann(int m, int n) {
    if (m == 0) {
        return n + 1;
    }
    if (n == 0) {
        return ackermann(m - 1, 1);
    }
    return ackermann(m - 1, ackermann(m, n - 1));
}

// Test 12: Recursion with multiple base cases
int tribonacci(int n) {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    if (n == 2) {
        return 1;
    }
    return tribonacci(n - 1) + tribonacci(n - 2) + tribonacci(n - 3);
}

// Test 13: Recursion with static variable
int recursive_counter(int n) {
    static int count = 0;
    count = count + 1;
    
    if (n <= 0) {
        return count;
    }
    return recursive_counter(n - 1);
}

// Test 14: Binary search (recursion with array)
int binary_search(int arr[], int left, int right, int target) {
    if (left > right) {
        return -1;
    }
    
    int mid = left + (right - left) / 2;
    
    if (arr[mid] == target) {
        return mid;
    }
    
    if (arr[mid] > target) {
        return binary_search(arr, left, mid - 1, target);
    }
    
    return binary_search(arr, mid + 1, right, target);
}

// Test 15: Recursion with conditional
int conditional_recursion(int n, int flag) {
    if (n <= 0) {
        return 0;
    }
    
    if (flag) {
        return n + conditional_recursion(n - 1, 1);
    } else {
        return n + conditional_recursion(n - 1, 0);
    }
}

// Test 16: Recursion in loop (Tower of Hanoi helper)
int count_moves(int n) {
    if (n == 1) {
        return 1;
    }
    return 2 * count_moves(n - 1) + 1;
}

// Test 17: Nested recursive calls
int nested_recursion(int n) {
    if (n > 100) {
        return n - 10;
    }
    return nested_recursion(nested_recursion(n + 11));
}

// Test 18: Recursion with pointer manipulation
int reverse_count(int *arr, int size) {
    if (size <= 0) {
        return 0;
    }
    return *arr + reverse_count(arr + 1, size - 1);
}

// Test 19: Mutual recursion with 3 functions
int func_a(int n) {
    if (n <= 0) return 1;
    if (n == 1) return 2;
    return func_a(n - 2);
}

int func_b(int n) {
    if (n <= 0) return 2;
    if (n == 1) return 3;
    return func_b(n - 2);
}

int func_c(int n) {
    if (n <= 0) return 3;
    if (n == 1) return 1;
    return func_c(n - 2);
}

// Test 20: Recursion with complex expression
int complex_recursive(int n) {
    if (n <= 1) {
        return 1;
    }
    return (n * complex_recursive(n - 1)) + (n - complex_recursive(n - 2));
}

// Test 21: Recursion with switch statement
int switch_recursion(int n, int op) {
    if (n <= 0) {
        return 0;
    }
    
    switch (op) {
        case 1:
            return n + switch_recursion(n - 1, 1);
        case 2:
            return n * switch_recursion(n - 1, 2);
        default:
            return switch_recursion(n - 1, 0);
    }
}

// Test 22: Recursion with while loop
int loop_recursion(int n) {
    if (n <= 0) {
        return 0;
    }
    
    int sum = 0;
    int i = n;
    while (i > 0) {
        sum = sum + 1;
        i = i - 1;
    }
    
    return sum + loop_recursion(n - 1);
}

// Test 23: Recursion returning pointer result
int helper_recursive(int n) {
    if (n <= 1) {
        return 1;
    }
    return n + helper_recursive(n - 1);
}

// Test 24: Deep recursion with multiple parameters
int multi_param_recursion(int a, int b, int c) {
    if (a <= 0) {
        return b + c;
    }
    return multi_param_recursion(a - 1, b + 1, c + 2);
}

// Test 25: Recursion with ternary operator
int ternary_recursion(int n) {
    return (n <= 1) ? 1 : n * ternary_recursion(n - 1);
}

int main() {
    int result = 0;
    int arr[5] = {1, 2, 3, 4, 5};
    char str[10] = "hello";
    
    // Test direct recursion
    result = factorial(5);
    result = fibonacci(7);
    result = sum_digits(12345);
    result = power(2, 10);
    
    // Test tail recursion
    result = factorial_tail(5);
    
    // Test with arrays
    result = array_sum(arr, 5);
    result = binary_search(arr, 0, 4, 3);
    
    // Test indirect recursion
    result = is_even(10);
    result = is_odd(9);
    
    // Test mutual recursion
    result = func_a(5);
    
    // Test special cases
    result = gcd(48, 18);
    result = countdown(10);
    result = ackermann(2, 2);
    result = tribonacci(6);
    
    // Test recursion with static
    result = recursive_counter(5);
    
    // Test complex recursion
    result = nested_recursion(95);
    result = complex_recursive(5);
    result = conditional_recursion(5, 1);
    result = count_moves(3);
    
    // Test recursion with control structures
    result = switch_recursion(3, 1);
    result = loop_recursion(3);
    result = multi_param_recursion(5, 0, 0);
    result = ternary_recursion(5);
    
    return 0;
}
