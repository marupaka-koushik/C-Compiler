// Comprehensive class and object testing

// Test 1: Simple class declaration
class Point {
    int x;
    int y;
};

// Test 2: Class with member functions
class Rectangle {
    int width;
    int height;
    
    int getArea() {
        return width * height;
    }
};

// Test 3: Class with constructor
class Circle {
    int radius;
    
    Circle(int r) {
        radius = r;
    }
    
    int getRadius() {
        return radius;
    }
};

// Test 4: Class with multiple methods
class Counter {
    int count;
    
    void increment() {
        count = count + 1;
    }
    
    void decrement() {
        count = count - 1;
    }
    
    int getValue() {
        return count;
    }
};

int main() {
    // Test 5: Create object and access members
    class Point p1;
    p1.x = 10;
    p1.y = 20;
    int a = p1.x;
    int b = p1.y;
    
    // Test 6: Create object and call method
    class Rectangle rect;
    rect.width = 50;
    rect.height = 30;
    int area = rect.getArea();
    
    // Test 7: Create object with constructor
    class Circle c1;
    c1.radius = 5;
    int r = c1.getRadius();
    
    // Test 8: Multiple objects of same class
    class Point p2;
    p2.x = 100;
    p2.y = 200;
    
    // Test 9: Object in expression
    int sum = p1.x + p2.x;
    
    // Test 10: Method call in expression
    class Counter cnt;
    cnt.count = 0;
    cnt.increment();
    int val = cnt.getValue();
    
    return 0;
}
