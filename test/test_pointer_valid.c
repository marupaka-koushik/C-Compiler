// Comprehensive test for pointers - VALID cases

int main() {
    // 1. Basic pointer declaration and initialization
    int x = 10;
    int *ptr;
    ptr = &x;
    
    // 2. Pointer dereferencing
    int y = *ptr;  // y = 10
    *ptr = 20;     // x = 20
    
    // 3. Pointer to pointer
    int **pptr;
    pptr = &ptr;
    int z = **pptr;  // z = 20
    
    // 4. Null pointer
    int *null_ptr = 0;
    
    // 5. Pointer arithmetic
    int arr[5] = {10, 20, 30, 40, 50};
    int *p = arr;  // Points to arr[0]
    p = p + 1;     // Now points to arr[1]
    int val = *p;  // val = 20
    
    // 6. Pointer increment/decrement
    p++;           // Points to arr[2]
    p--;           // Points to arr[1] again
    ++p;
    --p;
    
    // 7. Pointer difference
    int *p1 = &arr[0];
    int *p2 = &arr[3];
    int diff = p2 - p1;  // diff = 3
    
    // 8. Pointer comparison
    if (p1 < p2) {
        val = 1;
    }
    if (p1 == p1) {
        val = 2;
    }
    
    // 9. Array access via pointer
    int *parr = arr;
    parr[0] = 100;
    parr[1] = 200;
    val = parr[2];
    
    // 10. Pointer in loops
    int i;
    int *iter = arr;
    for (i = 0; i < 5; i = i + 1) {
        *iter = i * 10;
        iter = iter + 1;
    }
    
    // 11. Pointer to different types
    char c = 'A';
    char *cptr = &c;
    *cptr = 'B';
    
    float f = 3.14;
    float *fptr = &f;
    *fptr = 2.71;
    
    // 12. Multi-level pointers
    int a = 42;
    int *pa = &a;
    int **ppa = &pa;
    int ***pppa = &ppa;
    int value = ***pppa;  // value = 42
    
    // 13. Pointer assignment chains
    int b = 100;
    int *pb1 = &b;
    int *pb2 = pb1;
    int *pb3 = pb2;
    *pb3 = 200;  // b = 200
    
    // 14. Pointer in conditional expressions
    int *conditional_ptr = (x > 10) ? &x : &y;
    
    // 15. Pointer with struct-like usage (manual)
    int data[3];
    int *base = data;
    *(base + 0) = 1;
    *(base + 1) = 2;
    *(base + 2) = 3;
    
    // 16. Pointer swap
    int m = 5;
    int n = 10;
    int *pm = &m;
    int *pn = &n;
    int temp = *pm;
    *pm = *pn;
    *pn = temp;  // m=10, n=5
    
    // 17. Pointer to array element
    int matrix[3][3];
    int *pelem = &matrix[1][2];
    *pelem = 99;
    
    // 18. Pointer arithmetic with different offsets
    int nums[10];
    int *pnums = nums;
    *(pnums + 0) = 0;
    *(pnums + 5) = 50;
    *(pnums + 9) = 90;
    
    // 19. Character pointer (string)
    char str[] = "hello";
    char *s = str;
    char first = *s;      // 'h'
    char second = *(s+1); // 'e'
    
    // 20. Pointer in switch
    int choice = *ptr;
    switch(choice) {
        case 10:
            val = 1;
            break;
        case 20:
            val = 2;
            break;
        default:
            val = 0;
            break;
    }
    
    // 21. Multiple pointers to same variable
    int shared = 777;
    int *sp1 = &shared;
    int *sp2 = &shared;
    *sp1 = 888;
    val = *sp2;  // val = 888
    
    // 22. Pointer through array
    int traverse[5] = {1, 2, 3, 4, 5};
    int *t = traverse;
    int sum = 0;
    for (i = 0; i < 5; i = i + 1) {
        sum = sum + *(t + i);
    }
    
    // 23. Pointer with compound assignment
    int num = 100;
    int *pnum = &num;
    *pnum += 50;   // num = 150
    *pnum -= 20;   // num = 130
    *pnum *= 2;    // num = 260
    *pnum /= 4;    // num = 65
    
    // 24. Pointer with bitwise operations
    int bits = 0xFF;
    int *pbits = &bits;
    *pbits = *pbits & 0x0F;
    *pbits = *pbits | 0xF0;
    *pbits = *pbits ^ 0xAA;
    
    // 25. Void pointer (cast to int*)
    void *vptr = &x;
    int *iptr = vptr;
    val = *iptr;
    
    return 0;
}
