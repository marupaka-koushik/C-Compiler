// Multi-level pointers combined with structs, enums, and loops

enum Status {
    INIT,
    READY,
    RUNNING
};

struct Node {
    int value;
    enum Status status;
    struct Node *next;
};

int main() {
    // Test 1: Multi-level pointers with structs
    struct Node n1;
    n1.value = 100;
    n1.status = INIT;
    
    struct Node *pn1 = &n1;
    struct Node **ppn1 = &pn1;
    struct Node ***pppn1 = &ppn1;
    
    // Access through triple pointer
    (***pppn1).value = 200;
    int v1 = (**ppn1)->value;  // Should be 200
    
    // Test 2: Multi-level pointers with enum
    enum Status s = READY;
    enum Status *ps = &s;
    enum Status **pps = &ps;
    enum Status ***ppps = &pps;
    
    ***ppps = RUNNING;
    int statusVal = s;  // Should be 2 (RUNNING)
    
    // Test 3: Multi-level pointers in loops
    int arr[5] = {10, 20, 30, 40, 50};
    int *p = arr;
    int **pp = &p;
    int ***ppp = &pp;
    
    int sum = 0;
    int i = 0;
    until (i >= 5) {
        sum = sum + *(**ppp + i);
        i = i + 1;
    }
    
    // Test 4: Dynamic-like behavior with multi-level pointers
    int x = 5;
    int y = 10;
    int z = 15;
    
    int *ptrs[3];
    ptrs[0] = &x;
    ptrs[1] = &y;
    ptrs[2] = &z;
    
    int **pptrs = ptrs;
    int ***ppptrs = &pptrs;
    
    // Modify through triple pointer
    *(**ppptrs + 0) = 55;
    *(**ppptrs + 1) = 110;
    *(**ppptrs + 2) = 155;
    
    int total = x + y + z;  // Should be 320
    
    return 0;
}
