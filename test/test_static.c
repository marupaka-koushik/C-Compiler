// Test static keyword functionality

// Test 1: Static local variables (persist across function calls)
int counter() {
    static int count = 0;
    count++;
    return count;
}

// Test 2: Static global variables (internal linkage - file scope only)
static int globalStatic = 100;

static int helper() {
    return globalStatic + 10;
}

// Test 3: Static function (internal linkage)
static int privateFunction() {
    return 42;
}

// Test 4: Multiple static declarations
int multiCounter() {
    static int a = 1;
    static int b = 2;
    static int c = 3;
    return a + b + c;
}

// Test 5: Static with different types
int typeTest() {
    static int x = 10;
    static float y = 3.14;
    static char z = 'A';
    return x;
}

// Test 6: Static in nested blocks
int nestedStatic() {
    static int outer = 0;
    if (outer == 0) {
        static int inner = 5;
        inner++;
        outer = inner;
    }
    return outer;
}

int main() {
    // Call counter multiple times to test persistence
    int c1 = counter();  // Should be 1
    int c2 = counter();  // Should be 2
    int c3 = counter();  // Should be 3
    
    // Access static global
    int g = globalStatic;
    
    // Call static function
    int h = helper();
    int p = privateFunction();
    
    // Multiple static vars
    int m = multiCounter();
    
    // Different types
    int t = typeTest();
    
    // Nested static
    int n = nestedStatic();
    
    return 0;
}
