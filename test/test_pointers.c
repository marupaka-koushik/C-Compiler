int main() {
    // Test 1: Basic pointer declaration and assignment
    int x = 10;
    int *ptr = &x;
    int y = *ptr;
    
    // Test 2: Pointer arithmetic
    int arr[5] = {1, 2, 3, 4, 5};
    int *p = arr;
    int val1 = *(p + 1);  // Should get arr[1] = 2
    int val2 = *(p + 2);  // Should get arr[2] = 3
    
    // Test 3: Pointer increment/decrement
    p++;
    int val3 = *p;  // Should get arr[1] = 2
    p--;
    int val4 = *p;  // Should get arr[0] = 1
    
    // Test 4: Pointer difference
    int *p1 = &arr[0];
    int *p2 = &arr[3];
    int diff = p2 - p1;  // Should be 3
    
    // Test 5: Pointer comparison
    if (p1 < p2) {
        int valid = 1;
    }
    
    // Test 6: Pointer assignment through dereference
    *ptr = 20;
    int z = x;  // Should be 20
    
    // Test 7: Pointer to pointer
    int **pptr = &ptr;
    int w = **pptr;
    
    // Test 8: Null pointer
    int *nullPtr = 0;
    
    // Test 9: Array name as pointer
    int *arrPtr = arr;
    int val5 = arrPtr[2];
    
    // Test 10: Pointer indexing
    int val6 = ptr[0];
    
    return 0;
}
