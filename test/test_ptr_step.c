int main() {
    // Test 1: Basic pointer declaration and assignment
    int x = 10;
    int *ptr = &x;
    int y = *ptr;
    
    // Test 2: Pointer arithmetic
    int arr[5] = {1, 2, 3, 4, 5};
    int *p = arr;
    int val1 = *(p + 1);
    
    // Test 3: Pointer increment
    p++;
    
    return 0;
}
