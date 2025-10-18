// Comprehensive multi-level pointer test

int main() {
    // Test 1: Triple pointer
    int x = 42;
    int *p1 = &x;
    int **p2 = &p1;
    int ***p3 = &p2;
    
    // Test 2: Dereferencing triple pointer
    int val1 = ***p3;  // Should be 42
    
    // Test 3: Partial dereferencing
    int **partial2 = *p3;
    int *partial1 = *partial2;
    int value = *partial1;
    
    // Test 4: Quadruple pointer
    int ****p4 = &p3;
    int val2 = ****p4;  // Should be 42
    
    // Test 5: Assignment through multi-level pointers
    ***p3 = 100;
    int val3 = x;  // Should be 100
    
    // Test 6: Multi-level pointer with arrays
    int arr[3] = {1, 2, 3};
    int *parr[3];
    parr[0] = &arr[0];
    parr[1] = &arr[1];
    parr[2] = &arr[2];
    
    int **pparr = parr;
    int val4 = **pparr;  // Should be 1
    
    // Test 7: Pointer to pointer arithmetic
    int **pp = pparr;
    pp++;
    int val5 = **pp;  // Should be 2
    
    // Test 8: Mixed levels
    int a = 10;
    int *pa = &a;
    int **ppa = &pa;
    int ***pppa = &ppa;
    
    **ppa = 20;
    *pa = 30;
    ***pppa = 40;
    
    return 0;
}
