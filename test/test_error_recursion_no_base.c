// Error: Infinite recursion - no base case

int infinite_recursion(int n) {
    // Missing base case - will recurse forever
    return 1 + infinite_recursion(n - 1);
}

int main() {
    int result = infinite_recursion(5);
    return 0;
}
