// Test pass-by-reference simulation using pointers

// Function that modifies value via pointer (simulating reference)
void increment(int *ptr) {
    *ptr = *ptr + 1;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    // Test 1: Pass by reference (using address-of)
    int x = 10;
    increment(&x);
    int val1 = x;
    
    // Test 2: Swap two values
    int a = 5;
    int b = 15;
    swap(&a, &b);
    int val2 = a;
    int val3 = b;
    
    // Test 3: Multiple pointer "references"
    int num = 100;
    int *p = &num;
    *p = 200;
    int result = num;
    
    return 0;
}
