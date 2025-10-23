// Error: Static function parameter (not allowed)

int test_function(static int x) {  // Error: parameters cannot be static
    return x + 1;
}

int main() {
    int result = test_function(5);
    return 0;
}
