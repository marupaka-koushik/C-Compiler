// Minimal test for forward declaration
int foo(int x); // Forward declaration with parameter

int main() {
    int result = foo(5); // Should work
    return result;
}

int foo(int x) {
    return x * 2;
}
