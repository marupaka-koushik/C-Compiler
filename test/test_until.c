// Comprehensive until-loop testing

int main() {
    // Test 1: Simple until loop
    int i = 0;
    until (i >= 5) {
        i = i + 1;
    }
    
    // Test 2: until with decrement
    int j = 10;
    until (j <= 0) {
        j = j - 1;
    }
    
    // Test 3: until with complex condition
    int a = 0;
    int b = 10;
    until (a >= 5 || b <= 5) {
        a = a + 1;
        b = b - 1;
    }
    
    return 0;
}
