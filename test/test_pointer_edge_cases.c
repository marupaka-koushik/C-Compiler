// Pointer edge cases - Advanced patterns

int main() {
    // 1. Pointer to pointer chains
    int base = 100;
    int *p1 = &base;
    int **p2 = &p1;
    int ***p3 = &p2;
    int ****p4 = &p3;
    
    // Modify through 4-level indirection
    ****p4 = 200;
    int check = base;  // check = 200
    
    // 2. Simpler pointer array test
    int a = 10;
    int b = 20;
    int c = 30;
    int *pa = &a;
    int *pb = &b;
    int *pc = &c;
    
    // Access through pointers
    int sum = *pa + *pb + *pc;  // 60
    
    // 3. Pointer arithmetic with negative offsets
    int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *mid = &arr[5];
    int val1 = *(mid - 2);  // arr[3] = 3
    int val2 = *(mid + 2);  // arr[7] = 7
    
    // 4. Complex pointer expressions
    int x = 5;
    int y = 10;
    int *px = &x;
    int *py = &y;
    int result = (*px) * (*py);  // 50
    *px = *px + *py;  // x = 15
    
    // 5. Pointer in nested loops
    int matrix[3][3];
    int i, j;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            int *elem = &matrix[i][j];
            *elem = i * 3 + j;
        }
    }
    
    // 6. Pointer aliasing
    int original = 42;
    int *alias1 = &original;
    int *alias2 = &original;
    int *alias3 = alias1;
    
    *alias1 = 10;
    *alias2 = *alias2 + 5;  // original = 15
    *alias3 = *alias3 * 2;  // original = 30
    
    // 7. Pointer to const value (manual const)
    int const_val = 100;
    int *pconst = &const_val;
    int read = *pconst;
    
    // 8. Pointer walking through array
    int data[5] = {10, 20, 30, 40, 50};
    int *walker = data;
    int product = 1;
    for (i = 0; i < 5; i = i + 1) {
        product = product * (*walker);
        walker++;
    }
    
    // 9. Pointer with ternary operator
    int max_val = (*px > *py) ? *px : *py;
    int *max_ptr = (*px > *py) ? px : py;
    *max_ptr = 999;
    
    // 10. Pointer to function parameter (simulated)
    int param = 777;
    int *pparam = &param;
    *pparam = *pparam + 111;  // param = 888
    
    // 11. Multiple dereferencing in expression
    int ***triple_ptr = &p2;
    int value = ***triple_ptr + 10;
    
    // 12. Pointer comparison chains
    int *p_start = &arr[0];
    int *p_mid = &arr[5];
    int *p_end = &arr[9];
    
    int is_ordered = (p_start < p_mid) && (p_mid < p_end);
    
    // 13. Pointer with all arithmetic operations
    int *arith = &arr[5];
    arith = arith + 2;  // arr[7]
    arith = arith - 3;  // arr[4]
    int idx = arith - arr;  // 4
    
    // 14. Character pointer operations
    char text[] = "pointer";
    char *cp = text;
    while (*cp != '\0') {
        *cp = *cp - 32;  // Convert to uppercase
        cp++;
    }
    
    // 15. Pointer with simpler pattern
    int arr2[5] = {1, 2, 3, 4, 5};
    int *p_to_arr = arr2;
    
    for (i = 0; i < 5; i = i + 1) {
        int val = *(p_to_arr + i);
    }
    
    // 16. Pointer circular reference (simulated)
    int node1 = 100;
    int node2 = 200;
    int *pn1 = &node1;
    int *pn2 = &node2;
    
    // Swap pointers
    int *temp_ptr = pn1;
    pn1 = pn2;
    pn2 = temp_ptr;
    
    // 17. Pointer with complex indexing
    int grid[4][4];
    int *pg = &grid[0][0];
    *(pg + 0*4 + 0) = 1;   // grid[0][0]
    *(pg + 1*4 + 2) = 2;   // grid[1][2]
    *(pg + 3*4 + 3) = 3;   // grid[3][3]
    
    // 18. Pointer increment in expression
    int sequence[5] = {1, 2, 3, 4, 5};
    int *ps = sequence;
    int first = *ps++;   // first=1, ps points to sequence[1]
    int second = *++ps;  // ps points to sequence[2], second=3
    
    // 19. Pointer with logical operations
    int flag1 = 1;
    int flag2 = 0;
    int *pf1 = &flag1;
    int *pf2 = &flag2;
    int both = (*pf1) && (*pf2);  // 0
    int either = (*pf1) || (*pf2); // 1
    
    // 20. Double pointer simplified
    int values[3] = {10, 20, 30};
    int *pval = &values[1];
    int **ppval = &pval;
    
    int retrieved = **ppval;  // 20
    
    return 0;
}
