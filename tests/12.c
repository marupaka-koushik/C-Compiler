// Exhaustive Test Suite for Recursive Functions
// Features: Linear, Tree, and Mutual Recursion
// Integration: Static variables, Arrays, Arithmetic
// Returns: 0 on Success, Line Number on Failure

// --- Forward Declarations (for Mutual Recursion) ---
// If your compiler doesn't support these, move is_even above is_odd
int is_even(int n);
int is_odd(int n);

// ==========================================
// 1. Linear Recursion (Sum of 0..n)
// ==========================================
int recursive_sum(int n) {
    if (n <= 0) return 0;
    // Simple case: Stack just needs to hold 'n' and '$ra'
    return n + recursive_sum(n - 1);
}

// ==========================================
// 2. Tree Recursion (Fibonacci) - THE STRESS TEST
// ==========================================
// fib(n) = fib(n-1) + fib(n-2)
// This forces the compiler to save the result of fib(n-1) 
// onto the stack before calling fib(n-2).
int fib(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fib(n - 1) + fib(n - 2);
}

// ==========================================
// 3. Mutual Recursion (Ping-Pong)
// ==========================================
int is_odd(int n) {
    if (n == 0) return 0;
    return is_even(n - 1);
}

int is_even(int n) {
    if (n == 0) return 1;
    return is_odd(n - 1);
}

// ==========================================
// 4. Ackermann Function (Deep Recursion)
// ==========================================
// Grows very fast. Used to test deep stack depth.
// A(m, n)
int ackermann(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return ackermann(m - 1, 1);
    return ackermann(m - 1, ackermann(m, n - 1));
}

// ==========================================
// 5. Static Variable in Recursion
// ==========================================
// 'depth' should increment for every call, shared across frames.
int recursion_depth_counter(int n) {
    static int depth = 0;
    depth = depth + 1;
    
    if (n > 0) {
        recursion_depth_counter(n - 1);
    }
    return depth;
}

// ==========================================
// 6. Recursion with Arrays
// ==========================================
// Find max element recursively
int find_max(int* arr, int n) {
    if (n == 1) return arr[0];
    
    int sub_max = find_max(arr, n - 1);
    
    if (arr[n - 1] > sub_max) {
        return arr[n - 1];
    } else {
        return sub_max;
    }
}

int main() {
    int res = 0;

    // --- Test 1: Linear Recursion ---
    // Sum(5) = 5+4+3+2+1 = 15
    res = recursive_sum(5);
    if (res != 15) {
        return 92; // FAILED: Linear recursion
    }

    // --- Test 2: Tree Recursion (Fibonacci) ---
    // Fib(6) = 8
    // Sequence: 0, 1, 1, 2, 3, 5, 8
    // If this fails, your Register Preservation logic is broken.
    res = fib(6);
    if (res != 8) {
        return 101; // FAILED: Tree recursion (Register clobbering?)
    }

    // --- Test 3: Mutual Recursion ---
    // 11 is odd (1), 10 is even (1)
    if (is_odd(11) != 1) {
        return 107; // FAILED: Mutual recursion (is_odd)
    }
    if (is_even(10) != 1) {
        return 110; // FAILED: Mutual recursion (is_even)
    }

    // --- Test 4: Ackermann (Deep Stack) ---
    // A(2, 1) = 5
    // A(0, n) = n+1
    // A(1, n) = n+2
    // A(2, n) = 2*n + 3
    res = ackermann(2, 1); 
    if (res != 5) {
        return 120; // FAILED: Ackermann logic
    }

    // --- Test 5: Static in Recursion ---
    // Calling with n=4. 
    // Calls: n=4, n=3, n=2, n=1, n=0. (5 calls).
    // 'depth' should be 5.
    res = recursion_depth_counter(4);
    if (res != 5) {
        return 130; // FAILED: Static variable shared in recursion
    }

    // --- Test 6: Array Recursion ---
    int arr[5];
    arr[0] = 10; arr[1] = 50; arr[2] = 20; arr[3] = 90; arr[4] = 30;
    
    res = find_max(arr, 5);
    if (res != 90) {
        return 140; // FAILED: Recursive array processing
    }

    return 0; // ALL TESTS PASSED
}