// Comprehensive variable argument function testing

// Test 1: Simple variadic function declaration
int sum(int count, ...) {
    return count;
}

int main() {
    // Test call with different number of arguments
    int result = sum(3, 10, 20, 30);
    
    return 0;
}
