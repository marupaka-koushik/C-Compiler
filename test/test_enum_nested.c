// Test: Enum in different scopes
enum Color {
    RED,
    GREEN
};

int main() {
    enum Color c = RED;
    
    // Can we use same constant name in different context?
    int RED = 10;  // This should be an error or shadowing
    
    return c;
}
