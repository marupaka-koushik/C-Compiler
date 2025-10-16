// Advanced typedef test

// Test 1: Basic int typedef
typedef int Integer;

int test1() {
    Integer x = 10;
    return x;
}

// Test 2: Pointer typedef
typedef int* IntPtr;

int test2() {
    int val = 42;
    IntPtr p = &val;
    return *p;
}

// Test 3: Anonymous struct typedef
typedef struct {
    int x;
    int y;
} Point;

int test3() {
    Point p;
    p.x = 10;
    p.y = 20;
    return p.x + p.y;
}

// Test 4: Named struct typedef
struct Rect {
    int width;
    int height;
};

typedef struct Rect Rectangle;

int test4() {
    Rectangle r;
    r.width = 5;
    r.height = 10;
    return r.width * r.height;
}

// Test 5: Chained typedef
typedef int MyInt;
typedef MyInt Counter;

int test5() {
    Counter c = 100;
    return c;
}

int main() {
    int r = 0;
    r = test1();
    r = test2();
    r = test3();
    r = test4();
    r = test5();
    return 0;
}
