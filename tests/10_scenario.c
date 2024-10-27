// Test with exact scenario from test 10
int g_val = 0;

int sum_four(int a, int b, int c, int d) {
    return a + b + c + d;
}

int main() {
    int result = 0;
    int a = 10; 
    int b = 20;

    // --- Scenario 1: Basic Return & Args ($a0-$a3) ---
    result = sum_four(1, 2, 3, 4);
    if (result != 10) {
        return 96; // FAILED
    }

    // --- Scenario 2: Argument Evaluation Order ---
    result = sum_four(a + b, a - b, a + b, a - b);
    if (result != 40) {
        return 104; // FAILED: Complex expression args
    }

    return 0; // SUCCESS
}
