// Error Test 3: Type mismatch in pointer assignment
int main() {
    int x = 10;
    float* ptr = &x;  // Should error: type mismatch
    return 0;
}
