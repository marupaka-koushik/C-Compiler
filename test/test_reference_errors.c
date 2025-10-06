// Error Test 1: Dereferencing non-pointer
int main() {
    int x = 10;
    int y = *x;  // Should error: x is not a pointer
    return y;
}
