// Test expression arguments
int sum_four(int a, int b, int c, int d) {
    return a + b + c + d;
}

int main() {
    int a = 10;
    int b = 20;
    int result = sum_four(a + b, a - b, a + b, a - b);
    // Should be: 30 + (-10) + 30 + (-10) = 40
    return result; // Expecting 40
}
