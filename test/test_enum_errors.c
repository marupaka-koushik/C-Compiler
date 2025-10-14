// Error Test 1: Duplicate enum constant names within same enum
enum Test1 {
    A,
    B,
    A  // Should detect duplicate
};

int main() {
    return 0;
}
