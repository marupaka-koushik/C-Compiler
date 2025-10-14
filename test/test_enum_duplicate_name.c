// Error Test 2: Duplicate enum type names
enum Color {
    RED,
    GREEN
};

enum Color {  // Should detect duplicate enum type
    BLUE,
    YELLOW
};

int main() {
    return 0;
}
