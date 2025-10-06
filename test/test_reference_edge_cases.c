// Test 1: Multiple indirection levels
void modifyPointer(int** pptr) {
    int value = 100;
    *pptr = &value;
}

// Test 2: Array as reference
void fillArray(int* arr, int size) {
    int i;
    for (i = 0; i < size; i = i + 1) {
        arr[i] = i * 2;
    }
}

// Test 3: Return pointer
int* getPointer(int* x) {
    return x;
}

// Test 4: Pointer arithmetic
void addOffset(int* ptr, int offset) {
    *(ptr + offset) = 99;
}

int main() {
    int x = 10;
    int* px = &x;
    int** ppx = &px;
    
    // Test double pointer
    int value = **ppx;
    
    // Test array reference
    int arr[5];
    fillArray(arr, 5);
    
    // Test return pointer
    int* result = getPointer(&x);
    
    return value + arr[0];
}
