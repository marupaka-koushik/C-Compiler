// Test C++ reference syntax

// Test 1: Basic reference declaration and usage
void test_basic_ref() {
    int x = 10;
    int &ref = x;  // Reference to x
    ref = 20;      // Should modify x
    int y = ref;   // Should get value from x
}

// Test 2: Reference parameters
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void increment(int &n) {
    n = n + 1;
}

// Test 3: Reference return types
int& getElement(int arr[], int index) {
    return arr[index];
}

int& getMax(int &a, int &b) {
    if (a > b) {
        return a;
    }
    return b;
}

// Test 4: Const references
void printValue(const int &val) {
    int x = val;  // Read only
}

void processArray(const int arr[], int size) {
    int first = arr[0];
}

// Test 5: Multiple references
void multiRef(int &x, int &y, int &z) {
    x = 1;
    y = 2;
    z = 3;
}

// Test 6: Reference with pointers
void refToPointer(int* &ptr) {
    int temp = 99;
    ptr = &temp;
}

int main() {
    // Test basic reference
    int a = 5;
    int &r = a;
    r = 10;
    
    // Test reference parameters
    int p = 3;
    int q = 7;
    swap(p, q);
    
    increment(a);
    
    // Test reference return
    int arr[5] = {1, 2, 3, 4, 5};
    int &elem = getElement(arr, 2);
    elem = 99;
    
    // Test const reference
    printValue(a);
    
    // Test multiple refs
    int x = 0;
    int y = 0; 
    int z = 0;
    multiRef(x, y, z);
    
    return 0;
}
