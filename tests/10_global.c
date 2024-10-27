// Test with global variable
int g_val = 0;

int sum_four(int a, int b, int c, int d) {
    return a + b + c + d;
}

int main() {
    int result = sum_four(1, 2, 3, 4);
    return result;  // Should be 10
}
