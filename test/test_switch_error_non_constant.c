// ERROR TEST: Non-constant case expression
// This should produce a compilation error

int main() {
    int x = 2;
    int y = 5;
    int result = 0;
    
    // ERROR: Case with variable (non-constant expression)
    switch (x) {
        case 1:
            result = 10;
            break;
        case y:  // VARIABLE - should error (must be constant)
            result = 20;
            break;
        default:
            result = 0;
            break;
    }
    
    return result;
}
