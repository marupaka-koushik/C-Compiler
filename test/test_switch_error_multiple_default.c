// ERROR TEST: Multiple default labels
// This should produce a compilation error

int main() {
    int x = 2;
    int result = 0;
    
    // ERROR: Multiple default labels
    switch (x) {
        case 1:
            result = 10;
            break;
        default:
            result = 20;
            break;
        case 2:
            result = 30;
            break;
        default:  // DUPLICATE DEFAULT - should error
            result = 40;
            break;
    }
    
    return result;
}
