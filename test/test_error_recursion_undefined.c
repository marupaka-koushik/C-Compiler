// Error: Calling undefined recursive function

int helper(int n) {
    if (n <= 0) {
        return 0;
    }
    // Error: undefined_function is not declared
    return n + undefined_function(n - 1);
}

int main() {
    int result = helper(5);
    return 0;
}
