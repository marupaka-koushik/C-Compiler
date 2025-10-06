// Comprehensive reference/pointer test

// Test 1: Simple pass-by-reference
void doubleValue(int* ptr) {
    *ptr = *ptr * 2;
}

// Test 2: Multiple pointer parameters
void add(int* a, int* b, int* result) {
    *result = *a + *b;
}

// Test 3: Pointer in struct (if supported)
struct Point {
    int x;
    int y;
};

void movePoint(struct Point* p, int dx, int dy) {
    p->x = p->x + dx;
    p->y = p->y + dy;
}

// Test 4: Array manipulation
void reverseArray(int* arr, int size) {
    int i;
    int j = size - 1;
    for (i = 0; i < j; i = i + 1) {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
        j = j - 1;
    }
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
    
    // Test struct pointer
    struct Point pt;
    pt.x = 0;
    pt.y = 0;
    movePoint(&pt, 10, 5);
    
    // Test array reference
    int numbers[5];
    numbers[0] = 1;
    numbers[1] = 2;
    numbers[2] = 3;
    numbers[3] = 4;
    numbers[4] = 5;
    reverseArray(numbers, 5);
    
    return x + result + pt.x + numbers[0];
}
