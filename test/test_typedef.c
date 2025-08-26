// Test typedef functionality
typedef int Integer;
typedef float Real;
typedef struct Point {
    int x;
    int y;
} Point;

typedef struct {
    float x;
    float y;
} Vector;

typedef int* IntPtr;
typedef int Array[10];

// Using typedef'd types
Integer num = 42;
Real value = 3.14;
Point p1;
Vector v1;
IntPtr ptr;
Array arr;

int main() {
    Integer a = 10;
    Real b = 20.5;
    return 0;
}
