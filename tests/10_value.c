// Test what value we actually get
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
        return result; // Return what we got
    }

    // Test 2
    result = sum_four(a + b, a - b, a + b, a - b);
    return result; // Return what we got (should be 40)
}
