// Comprehensive test for structures - VALID cases

struct Point {
    int x;
    int y;
};

struct Rectangle {
    struct Point topLeft;
    struct Point bottomRight;
};

struct Person {
    char name[50];
    int age;
    float height;
};

struct Node {
    int data;
    struct Node *next;
};

int main() {
    // 1. Basic structure declaration and initialization
    struct Point p1;
    p1.x = 10;
    p1.y = 20;
    
    // 2. Structure with initializer
    struct Point p2 = {30, 40};
    
    // 3. Accessing structure members
    int x_val = p1.x;
    int y_val = p1.y;
    
    // 4. Structure assignment
    struct Point p3;
    p3 = p1;  // Copy all members
    
    // 5. Structure member in expressions
    int sum = p1.x + p1.y;
    int product = p2.x * p2.y;
    
    // 6. Structure member modification
    p1.x = p1.x + 5;
    p1.y++;
    p2.x--;
    
    // 7. Nested structures
    struct Rectangle rect;
    rect.topLeft.x = 0;
    rect.topLeft.y = 0;
    rect.bottomRight.x = 100;
    rect.bottomRight.y = 100;
    
    int width = rect.bottomRight.x - rect.topLeft.x;
    int height = rect.bottomRight.y - rect.topLeft.y;
    
    // 8. Structure with array members
    struct Person person1;
    person1.name[0] = 'J';
    person1.name[1] = 'o';
    person1.name[2] = 'h';
    person1.name[3] = 'n';
    person1.name[4] = '\0';
    person1.age = 25;
    person1.height = 5.9;
    
    // 9. Array of structures
    struct Point points[3];
    points[0].x = 1;
    points[0].y = 2;
    points[1].x = 3;
    points[1].y = 4;
    points[2].x = 5;
    points[2].y = 6;
    
    // 10. Structure in loops
    int i;
    for (i = 0; i < 3; i = i + 1) {
        points[i].x = i * 10;
        points[i].y = i * 20;
    }
    
    // 11. Structure member in conditional
    if (p1.x > p2.x) {
        sum = p1.x;
    } else {
        sum = p2.x;
    }
    
    // 12. Structure member in switch
    switch(p1.x) {
        case 10:
            sum = 1;
            break;
        case 20:
            sum = 2;
            break;
        default:
            sum = 0;
            break;
    }
    
    // 13. Pointer to structure
    struct Point *pptr;
    pptr = &p1;
    pptr->x = 99;
    pptr->y = 88;
    int val = pptr->x;
    
    // 14. Self-referential structure (linked list node) - Simple version
    struct Node node1;
    node1.data = 10;
    
    struct Node node2;
    node2.data = 20;
    
    // 15. Basic pointer operations
    struct Point *current;
    current = &p1;
    int xval = current->x;
    
    // 16. Structure member with all operators
    p1.x = p1.x + 10;
    p1.x = p1.x - 5;
    p1.x = p1.x * 2;
    p1.x = p1.x / 2;
    p1.x = p1.x % 3;
    
    // 17. Structure member compound assignment
    p1.x += 10;
    p1.y -= 5;
    p2.x *= 2;
    p2.y /= 2;
    
    // 18. Structure member with bitwise operations
    p1.x = p1.x & 0xFF;
    p1.x = p1.x | 0x10;
    p1.x = p1.x ^ 0xAA;
    p1.x = p1.x << 1;
    p1.x = p1.x >> 2;
    
    // 19. Structure member in ternary operator
    int max_x = (p1.x > p2.x) ? p1.x : p2.x;
    
    // 20. Multiple structure variables
    struct Point a, b, c;
    a.x = 1;
    a.y = 2;
    b.x = 3;
    b.y = 4;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    
    // 21. Structure comparison (manual)
    int same = 0;
    if (a.x == b.x && a.y == b.y) {
        same = 1;
    }
    
    // 22. Structure member address
    int *px;
    int *py;
    px = &(p1.x);
    py = &(p1.y);
    *px = 100;
    *py = 200;
    
    // 23. Nested structure access via pointer
    struct Rectangle *rectPtr;
    rectPtr = &rect;
    rectPtr->topLeft.x = 10;
    rectPtr->bottomRight.y = 20;
    
    // 24. Structure with mixed types
    struct Mixed {
        char c;
        int i;
        float f;
        double d;
    };
    
    struct Mixed m;
    m.c = 'A';
    m.i = 42;
    m.f = 3.14;
    m.d = 2.71;
    
    // 25. Structure array initialization
    struct Point grid[2][2];
    grid[0][0].x = 0;
    grid[0][0].y = 0;
    grid[1][1].x = 10;
    grid[1][1].y = 10;
    
    return 0;
}
