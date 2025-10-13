// Simple struct test

int main() {
    // Test 1: Basic struct declaration and member access
    struct Point {
        int x;
        int y;
    };
    
    struct Point p;
    p.x = 10;
    p.y = 20;
    
    int a = p.x;
    int b = p.y;
    
    return 0;
}
