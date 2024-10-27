// Debug test for expression arguments
int sum_four(int a, int b, int c, int d);

int main() {
    int result = 0;
    int a = 10; 
    int b = 20;

    // --- Scenario 1: Basic Return & Args ($a0-$a3) ---
    // 1 + 2 + 3 + 4 = 10
    result = sum_four(1, 2, 3, 4);
    if (result != 10) {
        return 96; // FAILED: Basic arg passing or return
    }

    // --- Scenario 2: Argument Evaluation Order ---
    // Should calculate (10+20) and (10-20) BEFORE calling
    // sum_four(30, -10, 30, -10) = 40
    result = sum_four(a + b, a - b, a + b, a - b);
    
    // Return actual result to see what we got
    return result; // Should be 40
}

int sum_four(int a, int b, int c, int d) {
    return a + b + c + d;
}
