// Test for basic pointer operations

int main() {
    int result = 0;
    
    // TEST 1: Pointer declaration and address-of
    int x = 10;
    int *p;
    p = &x;
    
    // TEST 2: Dereferencing pointer
    int y = *p;
    result = *p;
    
    // TEST 3: Modifying through pointer
    *p = 20;
    result = x;
    
    // TEST 4: Pointer to array
    int arr[5];
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    arr[3] = 4;
    arr[4] = 5;
    
    // TEST 5: Pointer to first element
    int *ptr;
    ptr = arr;
    
    // TEST 6: Dereference array pointer
    result = *ptr;
    
    // TEST 7: Pointer arithmetic - addition
    ptr = ptr + 1;
    result = *ptr;
    
    // TEST 8: Pointer arithmetic - subtraction
    ptr = ptr - 1;
    result = *ptr;
    
    // TEST 9: Pointer increment
    ptr = ptr + 1;
    ptr = ptr + 1;
    result = *ptr;
    
    // TEST 10: Address of array element
    int *p2;
    p2 = &arr[2];
    result = *p2;
    
    // TEST 11: Modify through pointer
    *p2 = 99;
    result = arr[2];
    
    // TEST 12: Character pointer
    char c = 'A';
    char *pc;
    pc = &c;
    char ch = *pc;
    
    // TEST 13: Modify char through pointer
    *pc = 'B';
    result = c;
    
    // TEST 14: Pointer in expression
    int a = 5;
    int b = 10;
    int *pa;
    int *pb;
    pa = &a;
    pb = &b;
    
    result = *pa + *pb;
    
    // TEST 15: Pointer swap
    int temp = *pa;
    *pa = *pb;
    *pb = temp;
    
    result = a;
    result = b;
    
    // TEST 16: Pointer with array access notation
    int *p3;
    p3 = arr;
    result = p3[0];
    result = p3[2];
    
    // TEST 17: Dereferencing with offset
    result = *(arr + 3);
    
    // TEST 18: Complex expression
    int *p4;
    p4 = arr + 2;
    result = *p4;
    
    // TEST 19: Pointer in loop
    int *p5;
    p5 = arr;
    int sum = 0;
    for (int i = 0; i < 5; i = i + 1) {
        sum = sum + *p5;
        p5 = p5 + 1;
    }
    result = sum;
    
    // TEST 20: Pointer comparison
    int *p6;
    int *p7;
    p6 = arr;
    p7 = arr + 3;
    
    if (p6 < p7) {
        result = 100;
    }
    
    return result;
}
