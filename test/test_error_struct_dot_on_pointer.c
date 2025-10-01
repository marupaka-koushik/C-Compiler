// Error: Using dot operator on pointer

struct Point {
    int x;
    int y;
};

int main() {
    struct Point p;
    p.x = 10;
    p.y = 20;
    
    struct Point *ptr = &p;
    
    // ERROR: ptr is a pointer, should use -> instead of .
    int val = ptr.x;
    
    return 0;
}
