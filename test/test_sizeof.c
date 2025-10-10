// Test sizeof operator

int main() {
    // Test 1: sizeof with type
    int size1 = sizeof(int);
    
    // Test 2: sizeof with variable
    int x = 10;
    int size2 = sizeof(x);
    
    // Test 3: sizeof in expression
    int size3 = 5 * sizeof(int);
    
    // Test 4: sizeof with char
    int size4 = sizeof(char);
    
    // Test 5: sizeof in array
    int total = 10 * sizeof(int);
    
    return 0;
}
