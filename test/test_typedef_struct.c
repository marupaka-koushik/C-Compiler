// Test 3: Typedef with structs
struct Point {
    int x;
    int y;
};

typedef struct Point Point_t;

typedef struct {
    double real;
    double imag;
} Complex;

int main() {
    Point_t p1;
    p1.x = 10;
    p1.y = 20;
    
    Complex c;
    c.real = 3.0;
    c.imag = 4.0;
    
    return 0;
}
