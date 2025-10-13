// Structure edge cases - Simplified version

struct Point {
    int x;
    int y;
};

struct Employee {
    int id;
    int salary;
};

int main() {
    // 1. Deep nesting of structures
    struct A {
        int x;
    };
    struct B {
        struct A a;
        int y;
    };
    struct C {
        struct B b;
        int z;
    };
    
    struct C obj;
    obj.b.a.x = 10;
    obj.b.y = 20;
    obj.z = 30;
    int deep_val = obj.b.a.x + obj.b.y + obj.z;
    
    // 2. Array of structures
    struct Point points[5];
    int i;
    for (i = 0; i < 5; i = i + 1) {
        points[i].x = i * 10;
        points[i].y = i * 20;
    }
    
    // 3. Structure operations in loop
    int sum_x = 0;
    for (i = 0; i < 5; i = i + 1) {
        sum_x = sum_x + points[i].x;
    }
    
    // 4. 2D array of structures
    struct Point grid[3][3];
    int j;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            grid[i][j].x = i;
            grid[i][j].y = j;
        }
    }
    
    // 5. Structure with arithmetic on members
    struct Point p1, p2;
    p1.x = 10;
    p1.y = 20;
    p2.x = 30;
    p2.y = 40;
    
    int dist_x = p2.x - p1.x;
    int dist_y = p2.y - p1.y;
    
    // 6. Structure members in conditions
    int max_x = (p1.x > p2.x) ? p1.x : p2.x;
    
    // 7. Structure array search
    struct Employee employees[5];
    for (i = 0; i < 5; i = i + 1) {
        employees[i].id = i + 1;
        employees[i].salary = (i + 1) * 1000;
    }
    
    int search_id = 3;
    int found_salary = 0;
    for (i = 0; i < 5; i = i + 1) {
        if (employees[i].id == search_id) {
            found_salary = employees[i].salary;
            break;
        }
    }
    
    // 8. Nested structure access
    struct Pair {
        struct Point p1;
        struct Point p2;
    };
    
    struct Pair pair;
    pair.p1.x = 1;
    pair.p1.y = 2;
    pair.p2.x = 3;
    pair.p2.y = 4;
    
    int pair_sum = pair.p1.x + pair.p1.y + pair.p2.x + pair.p2.y;
    
    // 9. Bitwise operations on struct members
    struct Point bitwise;
    bitwise.x = 15;  // 0b1111
    bitwise.y = 10;  // 0b1010
    
    int and_result = bitwise.x & bitwise.y;
    int or_result = bitwise.x | bitwise.y;
    int xor_result = bitwise.x ^ bitwise.y;
    
    // 10. Pointer to structure
    struct Point *ptr;
    ptr = &p1;
    ptr->x = 99;
    ptr->y = 88;
    
    // 11. Pointer arithmetic with struct array - simplified
    struct Point arr[5];
    arr[0].x = 100;
    arr[1].x = 200;
    arr[2].x = 300;
    
    // 12. Structure member address
    int *px;
    px = &(p1.x);
    *px = 777;
    
    // 13. Nested structure pointer access
    struct Rect {
        struct Point topLeft;
        struct Point bottomRight;
    };
    
    struct Rect rect;
    struct Rect *rectPtr;
    rectPtr = &rect;
    rectPtr->topLeft.x = 10;
    rectPtr->topLeft.y = 20;
    rectPtr->bottomRight.x = 100;
    rectPtr->bottomRight.y = 200;
    
    // 14. Complex member expressions
    struct Point calc;
    calc.x = 5;
    calc.y = 10;
    
    int expr1 = calc.x * 2 + calc.y / 2;
    int expr2 = (calc.x + calc.y) * (calc.x - calc.y);
    
    // 15. Structure members in switch
    struct Point sw;
    sw.x = 2;
    
    int switch_result;
    switch (sw.x) {
        case 1:
            switch_result = 10;
            break;
        case 2:
            switch_result = 20;
            break;
        default:
            switch_result = 0;
            break;
    }
    
    return 0;
}
