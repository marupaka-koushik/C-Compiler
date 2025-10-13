// Structure edge cases - Advanced patterns

struct Employee {
    int id;
    int salary;
    struct Employee *manager;
};

struct TreeNode {
    int value;
    struct TreeNode *left;
    struct TreeNode *right;
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
    
    // 2. Structure with pointer members - simplified
    struct Employee emp1, emp2;
    emp1.id = 1;
    emp2.id = 2;
    
    // Just show basic member access
    int eid = emp1.id + emp2.id;
    
    // 3. Array of structures with integer operations
    struct IntPair {
        int a;
        int b;
    };
    
    struct IntPair numbers[5];
    int i;
    for (i = 0; i < 5; i = i + 1) {
        numbers[i].a = i * 10;
        numbers[i].b = i * 20;
    }
    
    // Pair addition
    struct IntPair sum;
    sum.a = numbers[0].a + numbers[1].a;
    sum.b = numbers[0].b + numbers[1].b;
    
    // 4. Binary tree operations - simplified
    struct TreeNode root, left, right;
    root.value = 10;
    left.value = 5;
    right.value = 15;
    
    // Just compute sum without pointer traversal
    int tree_sum = root.value + left.value + right.value;
    
    // 5. Structure with vector-like behavior
    struct Point {
        int x;
        int y;
    };
    
    struct Point p1, p2, result;
    p1.x = 3;
    p1.y = 4;
    p2.x = 5;
    p2.y = 6;
    
    // Vector addition
    result.x = p1.x + p2.x;
    result.y = p1.y + p2.y;
    
    // Dot product
    int dot = p1.x * p2.x + p1.y * p2.y;
    
    // 6. Structure array sorting (bubble sort)
    struct Point points[3];
    points[0].x = 30;
    points[1].x = 10;
    points[2].x = 20;
    
    int j, k;
    for (j = 0; j < 3; j = j + 1) {
        for (k = 0; k < 2; k = k + 1) {
            if (points[k].x > points[k+1].x) {
                struct Point temp = points[k];
                points[k] = points[k+1];
                points[k+1] = temp;
            }
        }
    }
    
    // 7. Structure with all integer types
    struct AllInts {
        char c;
        short s;
        int i;
        long l;
        int *ptr;
    };
    
    struct AllInts all;
    all.c = 90;
    all.s = 100;
    all.i = 1000;
    all.l = 100000;
    all.ptr = &(all.i);
    
    // 8. Pointer arithmetic with structure arrays
    struct Point arr[5];
    struct Point *p = arr;
    p->x = 10;
    (p + 1)->x = 20;
    (p + 2)->x = 30;
    
    // 9. Structure member swap
    struct Point swap_test;
    swap_test.x = 100;
    swap_test.y = 200;
    int temp_val = swap_test.x;
    swap_test.x = swap_test.y;
    swap_test.y = temp_val;
    
    // 10. Chained structure pointer access
    struct Node {
        int data;
        struct Node *next;
    };
    
    struct Node n1, n2;
    n1.data = 1;
    n2.data = 2;
    
    // Simplified - avoid self-referential pointer chains
    int node_sum = n1.data + n2.data;
    
    // 11. Structure with conditional operations
    struct Point conditional;
    conditional.x = 50;
    conditional.y = 60;
    
    int max_coord = (conditional.x > conditional.y) ? conditional.x : conditional.y;
    
    // 12. Structure array search
    struct Employee employees[5];
    for (i = 0; i < 5; i = i + 1) {
        employees[i].id = i + 1;
    }
    
    int search_id = 3;
    int found_index = 0;
    for (i = 0; i < 5; i = i + 1) {
        if (employees[i].id == search_id) {
            found_index = i;
            break;
        }
    }
    
    // 13. Nested structure array
    struct Pair {
        struct Point p1;
        struct Point p2;
    };
    
    struct Pair pairs[2];
    pairs[0].p1.x = 1;
    pairs[0].p1.y = 2;
    pairs[0].p2.x = 3;
    pairs[0].p2.y = 4;
    
    // 14. Structure with bitwise flags
    struct Flags {
        int bit0;
        int bit1;
        int bit2;
    };
    
    struct Flags flags;
    flags.bit0 = 1;
    flags.bit1 = 0;
    flags.bit2 = 1;
    
    int combined = (flags.bit0 << 0) | (flags.bit1 << 1) | (flags.bit2 << 2);
    
    // 15. Structure copy in loop
    struct Point source[3];
    struct Point dest[3];
    
    for (i = 0; i < 3; i = i + 1) {
        source[i].x = i;
        source[i].y = i * 2;
    }
    
    for (i = 0; i < 3; i = i + 1) {
        dest[i] = source[i];
    }
    
    return 0;
}
