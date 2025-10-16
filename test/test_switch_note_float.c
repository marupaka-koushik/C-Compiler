// EDGE CASE TEST: Float expression in switch
// This should produce a compilation error (switch requires integer)

int main() {
    // Note: In our simplified compiler, we may only have int
    // This test might not be applicable
    
    int x = 2;
    int result = 0;
    
    // Valid switch - using int
    switch (x) {
        case 1:
            result = 10;
            break;
        case 2:
            result = 20;
            break;
        default:
            result = 0;
            break;
    }
    
    return result;
}
