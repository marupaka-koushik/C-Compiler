// ERROR TEST: Duplicate case labels
// This should produce a compilation error

int main() {
    int x = 2;
    int result = 0;
    
    // ERROR: Duplicate case 2
    switch (x) {
        case 1:
            result = 10;
            break;
        case 2:
            result = 20;
            break;
        case 2:  // DUPLICATE - should error
            result = 30;
            break;
        default:
            result = 0;
            break;
    }
    
    return result;
}
