// Error: Recursive call with wrong number of parameters

int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    // Error: calling with 2 arguments when function expects 1
    return n * factorial(n - 1, 0);
}

int main() {
    int result = factorial(5);
    return 0;
}
