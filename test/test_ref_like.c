// Test address-of and dereferencing (without pointer parameters)

int main() {
    // Test 1: Basic address-of and dereference
    int x = 42;
    int *ptr = &x;
    int val = *ptr;
    
    // Test 2: Modify via pointer
    *ptr = 100;
    int newVal = x;
    
    // Test 3: Multiple pointers to same variable
    int *p1 = &x;
    int *p2 = &x;
    *p1 = 50;
    int result1 = *p2;
    
    // Test 4: Pointer to another pointer's target
    int y = 25;
    int *py = &y;
    *py = 30;
    int result2 = y;
    
    // Test 5: Chain of operations
    int a = 10;
    int *pa = &a;
    *pa = *pa + 5;
    int result3 = a;
    
    return 0;
}
