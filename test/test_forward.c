// Test forward declaration
int helper(int n);

int main() {
    int x = helper(5);
    return 0;
}

int helper(int n) {
    return n * 2;
}
