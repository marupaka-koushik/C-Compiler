// Comprehensive reference testing (C++ feature)

// Test 1: Simple reference variable
int main() {
    int x = 10;
    int &ref = x;
    
    // Test 2: Modify through reference
    ref = 20;
    int val = x;
    
    // Test 3: Reference in expression
    int y = ref + 5;
    
    // Test 4: Multiple references to same variable
    int &ref2 = x;
    ref2 = 30;
    
    return 0;
}
