// Comprehensive typedef with lexer feedback test

typedef int Integer;
typedef float Real;
typedef int* IntPtr;
typedef int IntArray[10];

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct {
    float x;
    float y;
} Vector;

// Using typedef'd types
Integer num;
Real value;
IntPtr ptr;
IntArray arr;

Point p1;
Point p2;
Vector v1;

int main() {
    Integer a;
    Integer b;
    Real c;
    Point myPoint;
    Vector myVector;
    IntPtr myPtr;
    
    a = 10;
    b = 20;
    c = 3.14;
    
    myPoint.x = 5;
    myPoint.y = 10;
    
    return 0;
}
