int g_val = 0;

int sum_four(int a, int b, int c, int d) {
    return a + b + c + d;
}

int main() {
    int result = 0;
    int a = 10; 
    int b = 20;

    // Test 1
    result = sum_four(1, 2, 3, 4);
    if (result != 10) {
        return 96;
    }

    // Test 2 - return the actual result to see what we get
    result = sum_four(a + b, a - b, a + b, a - b);
    return result; // Should be 40
}
