// Comprehensive struct testing

int main() {
    // Test 1: Simple struct declaration and member access
    struct Point {
        int x;
        int y;
    };
    struct Point p1;
    p1.x = 10;
    p1.y = 20;
    int a = p1.x;
    int b = p1.y;

    // Test 2: Struct with multiple data types
    struct Person {
        int age;
        char initial;
        int height;
    };
    struct Person person1;
    person1.age = 25;
    person1.initial = 'A';
    person1.height = 180;

    // Test 3: Nested struct member access
    struct Date {
        int day;
        int month;
        int year;
    };
    struct Employee {
        int id;
        struct Date joinDate;
    };
    struct Employee emp;
    emp.id = 101;
    emp.joinDate.day = 15;
    emp.joinDate.month = 6;
    emp.joinDate.year = 2023;

    // Test 4: Struct in expressions
    struct Rectangle {
        int width;
        int height;
    };
    struct Rectangle rect;
    rect.width = 50;
    rect.height = 30;
    int area = rect.width * rect.height;
    int perimeter = 2 * (rect.width + rect.height);

    // Test 5: Multiple struct variables
    struct Circle {
        int radius;
        int centerX;
        int centerY;
    };
    struct Circle c1;
    struct Circle c2;
    c1.radius = 5;
    c1.centerX = 0;
    c1.centerY = 0;
    c2.radius = 10;
    c2.centerX = 3;
    c2.centerY = 4;

    // Test 6: Struct member in conditions
    struct Student {
        int marks;
        int rollNo;
    };
    struct Student s1;
    s1.marks = 85;
    s1.rollNo = 42;
    if (s1.marks > 80) {
        int grade = 1;
    }

    // Test 7: Struct member in loops
    struct Counter {
        int start;
        int end;
    };
    struct Counter cnt;
    cnt.start = 0;
    cnt.end = 5;
    int i;
    for (i = cnt.start; i < cnt.end; i = i + 1) {
        int val = i;
    }

    // Test 8: Struct assignment chain
    struct Pair {
        int first;
        int second;
    };
    struct Pair pair;
    pair.first = 100;
    pair.second = pair.first + 50;

    // Test 9: Array of structs
    struct Point points[3];
    points[0].x = 1;
    points[0].y = 2;
    points[1].x = 3;
    points[1].y = 4;
    points[2].x = 5;
    points[2].y = 6;

    // Test 10: Struct with array member
    struct Data {
        int values[5];
        int count;
    };
    struct Data data;
    data.count = 3;
    data.values[0] = 10;
    data.values[1] = 20;
    data.values[2] = 30;

    // Test 11: Struct members in arithmetic operations
    struct Vector {
        int x;
        int y;
        int z;
    };
    struct Vector v1;
    struct Vector v2;
    v1.x = 1;
    v1.y = 2;
    v1.z = 3;
    v2.x = 4;
    v2.y = 5;
    v2.z = 6;
    int dotProduct = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

    // Test 12: Struct member modification in loop
    struct Box {
        int length;
        int width;
        int height;
    };
    struct Box box;
    box.length = 10;
    box.width = 10;
    box.height = 10;
    int j;
    for (j = 0; j < 3; j = j + 1) {
        box.length = box.length + 1;
    }

    // Test 13: Nested loops with struct members
    struct Grid {
        int rows;
        int cols;
    };
    struct Grid grid;
    grid.rows = 3;
    grid.cols = 4;
    int r;
    int c;
    for (r = 0; r < grid.rows; r = r + 1) {
        for (c = 0; c < grid.cols; c = c + 1) {
            int cell = r * grid.cols + c;
        }
    }

    // Test 14: Struct member comparison
    struct Range {
        int min;
        int max;
    };
    struct Range range;
    range.min = 10;
    range.max = 100;
    int value = 50;
    if (value >= range.min && value <= range.max) {
        int valid = 1;
    }

    // Test 15: Pointer to struct
    struct Node {
        int data;
        int next;
    };
    struct Node node;
    struct Node *ptr;
    node.data = 42;
    ptr = &node;
    int val2 = ptr->data;

    // Test 16: Struct member with compound assignment
    struct Account {
        int balance;
        int transactions;
    };
    struct Account acc;
    acc.balance = 1000;
    acc.transactions = 5;
    acc.balance += 500;
    acc.transactions++;

    // Test 17: Struct in while loop
    struct Timer {
        int seconds;
    };
    struct Timer timer;
    timer.seconds = 10;
    while (timer.seconds > 0) {
        timer.seconds = timer.seconds - 1;
    }

    // Test 18: Multiple nested structs
    struct Time {
        int hour;
        int minute;
    };
    struct Appointment {
        struct Date date;
        struct Time time;
    };
    struct Appointment apt;
    apt.date.day = 25;
    apt.date.month = 12;
    apt.date.year = 2024;
    apt.time.hour = 14;
    apt.time.minute = 30;

    // Test 19: Struct member in function-like expressions
    struct Stats {
        int min;
        int max;
        int avg;
    };
    struct Stats stats;
    stats.min = 5;
    stats.max = 95;
    stats.avg = (stats.min + stats.max) / 2;

    // Test 20: Complex struct operations
    struct Complex {
        int real;
        int imag;
    };
    struct Complex z1;
    struct Complex z2;
    struct Complex result;
    z1.real = 3;
    z1.imag = 4;
    z2.real = 1;
    z2.imag = 2;
    result.real = z1.real + z2.real;
    result.imag = z1.imag + z2.imag;

    return 0;
}
