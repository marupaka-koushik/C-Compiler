// Debug test for expression arguments
int sum_four(int a, int b, int c, int d);
void printf(const char* fmt, ...);

int main() {
    int result = 0;
    int a = 10; 
    int b = 20;

    // --- Scenario 1: Basic Return & Args ($a0-$a3) ---
    // 1 + 2 + 3 + 4 = 10
    result = sum_four(1, 2, 3, 4);
    printf("Test 1 result: %d (expected 10)\n", result);
    if (result != 10) {
        return 96; // FAILED: Basic arg passing or return
    }

    // --- Scenario 2: Argument Evaluation Order ---
    // Should calculate (10+20) and (10-20) BEFORE calling
    // sum_four(30, -10, 30, -10) = 40
    result = sum_four(a + b, a - b, a + b, a - b);
    printf("Test 2 result: %d (expected 40)\n", result);
    if (result != 40) {
        return 104; // FAILED: Complex expression args
    }

    return 0; // SUCCESS
}

int sum_four(int a, int b, int c, int d) {
    return a + b + c + d;
}
