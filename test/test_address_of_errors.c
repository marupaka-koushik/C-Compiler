// Error Test 2: Address of non-lvalue
int main() {
    int* ptr = &(10 + 20);  // Should error: cannot take address of expression
    return 0;
}
