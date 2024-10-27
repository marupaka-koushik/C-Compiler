// Exhaustive Test Suite for Structures
// Integration: Pointers, Arrays, Loops, Logic, Switch
// Returns: 0 on Success, Line Number on Failure

// --- Definition of Structs ---

struct Point {
    int x;
    int y;
};

struct Mixed {
    int id;
    char grade; // Should take 1 byte (plus padding usually)
    int score;  // Should be at offset 8 (if 4-byte aligned) or 5 (if packed)
};

struct Rect {
    struct Point p1; // Nested Struct
    struct Point p2;
};

// Linked List Node
struct Node {
    int val;
    struct Node* next; // Pointer to same struct type
};

struct ArrayHolder {
    int data[5]; // Array inside struct
};

int main() {
    int check = 0;
    int i = 0;

    // ============================================================
    // SECTION 1: BASIC STRUCT ACCESS (.)
    // ============================================================

    struct Point pt;
    pt.x = 10;
    pt.y = 20;

    // Test 1.1: Read back values
    if (pt.x != 10) {
        return 46; // FAILED: Struct member read (first member)
    }
    if (pt.y != 20) {
        return 49; // FAILED: Struct member read (second member)
    }

    // Test 1.2: Struct Member Arithmetic
    // 10 + 20 = 30
    check = pt.x + pt.y;
    if (check != 30) {
        return 56; // FAILED: Arithmetic with struct members
    }

    // ============================================================
    // SECTION 2: POINTERS TO STRUCTS (->)
    // ============================================================

    struct Point* ptr = &pt;

    // Test 2.1: Arrow Operator Read
    if (ptr->x != 10) {
        return 67; // FAILED: Struct pointer access (->)
    }

    // Test 2.2: Arrow Operator Write
    ptr->y = 99;
    if (pt.y != 99) {
        return 73; // FAILED: Struct pointer write side-effect
    }

    // Test 2.3: Dereference then Dot (*ptr).x
    // This tests operator precedence and mixed access
    if ((*ptr).x != 10) {
        return 79; // FAILED: (*ptr).member access
    }

    // ============================================================
    // SECTION 3: NESTED STRUCTURES
    // ============================================================

    struct Rect r;
    
    // Layout: 
    // r.p1 (offset 0) -> .x (0), .y (4)
    // r.p2 (offset 8) -> .x (8), .y (12)

    r.p1.x = 1;
    r.p1.y = 2;
    r.p2.x = 3;
    r.p2.y = 4;

    // Test 3.1: Nested Access
    if (r.p2.x != 3) {
        return 98; // FAILED: Nested struct access
    }

    // Test 3.2: Pointer to Nested
    struct Rect* rptr = &r;
    if (rptr->p1.y != 2) {
        return 104; // FAILED: Pointer -> Nested -> Member
    }

    // ============================================================
    // SECTION 4: ARRAYS INSIDE STRUCTS
    // ============================================================

    struct ArrayHolder h;
    
    // Test 4.1: Populate array inside struct
    for (i = 0; i < 5; i = i + 1) {
        h.data[i] = i * 10;
    }
    
    // Test 4.2: Read array inside struct
    if (h.data[3] != 30) {
        return 120; // FAILED: Accessing array inside struct
    }

    // ============================================================
    // SECTION 5: ARRAYS OF STRUCTS
    // ============================================================
    // This is CRITICAL. It checks if sizeof(struct Point) is calculated correctly.
    // If Point is size 8:
    // pts[0] at base+0
    // pts[1] at base+8
    // pts[2] at base+16

    struct Point pts[3];
    
    pts[0].x = 100;
    pts[1].x = 200;
    pts[2].x = 300;

    // Test 5.1: Access middle element
    if (pts[1].x != 200) {
        return 140; // FAILED: Array of Structs (Size calc error?)
    }

    // Test 5.2: Pointer Arithmetic on Structs
    struct Point* p_iter = &pts[0];
    p_iter = p_iter + 1; // Should jump 8 bytes (sizeof Point)
    
    if (p_iter->x != 200) {
        return 148; // FAILED: Pointer arithmetic on Struct type
    }

    // ============================================================
    // SECTION 6: LINKED LIST (Self-Referencing Structs)
    // ============================================================
    // Tests: Structs, Pointers, While Loops, NULL logic

    struct Node n1;
    struct Node n2;
    struct Node n3;

    n1.val = 1;
    n2.val = 2;
    n3.val = 3;

    // Link them: n1 -> n2 -> n3 -> NULL
    n1.next = &n2;
    n2.next = &n3;
    n3.next = 0; // NULL

    // Traverse
    struct Node* curr = &n1;
    int sum = 0;
    
    while (curr != 0) {
        sum = sum + curr->val;
        curr = curr->next;
    }
    
    // Sum = 1 + 2 + 3 = 6
    if (sum != 6) {
        return 177; // FAILED: Linked List traversal
    }

    // ============================================================
    // SECTION 7: MIXED TYPES (Alignment & Padding)
    // ============================================================

    struct Mixed m;
    m.id = 1;      // Offset 0
    m.grade = 'A'; // Offset 4 (assuming int takes 4 bytes)
    m.score = 100; // Offset 8 (due to alignment)
                   // If your compiler packs tightly, this might be offset 5.
                   // Most standard MIPS compilers align to 4 bytes.
    
    if (m.grade != 'A') {
        return 190; // FAILED: Char in struct
    }
    if (m.score != 100) {
        return 193; // FAILED: Int after Char in struct (Alignment issue?)
    }

    // ============================================================
    // SECTION 8: COMPLEX INTEGRATION
    // ============================================================

    // Switch case on struct member inside a loop
    pts[0].y = 0;
    pts[1].y = 1;
    pts[2].y = 2;
    
    check = 0;
    for (i = 0; i < 3; i = i + 1) {
        switch (pts[i].y) {
            case 0: check = check + 10; break;
            case 1: check = check + 20; break;
            case 2: check = check + 30; break;
        }
    }
    // 10 + 20 + 30 = 60
    if (check != 60) {
        return 213; // FAILED: Switch on struct member in loop
    }

    return 0; // ALL TESTS PASSED
}