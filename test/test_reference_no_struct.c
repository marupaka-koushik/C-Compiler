// Comprehensive reference test without struct pointer

// Test 1: Simple pass-by-reference
void doubleValue(int* ptr) {
    *ptr = *ptr * 2;
}

// Test 2: Multiple pointer parameters
void add(int* a, int* b, int* result) {
    *result = *a + *b;
}

// Test 3: Swap values
void swap(int* x, int* y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

// Test 4: Array manipulation
void fillArray(int* arr, int size, int value) {
    int i;
    for (i = 0; i < size; i = i + 1) {
        arr[i] = value + i;
    }
}

// Test 5: Nested pointer dereference
void setViaDoublePointer(int** pp, int value) {
    **pp = value;
}

int main() {
    // Test simple reference
    int x = 5;
    doubleValue(&x);
    
    // Test multiple references
    int a = 10;
    int b = 20;
    int result;
    add(&a, &b, &result);
    
    // Test swap
    int m = 100;
    int n = 200;
    swap(&m, &n);
    
    // Test array reference
    int numbers[5];
    fillArray(numbers, 5, 1);
    
    // Test double pointer
    int value = 50;
    int* pv = &value;
    setViaDoublePointer(&pv, 999);
    
    return x + result + m + numbers[0] + value;
}
