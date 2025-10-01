// Test with array notation (no pointer)

int main(int argc, int argv[]) {
    // Test 1: Access argc
    int count = argc;
    
    // Test 2: Access argv as array
    int first = argv[0];
    int second = argv[1];
    
    // Test 3: Loop through argv
    int i;
    for (i = 0; i < argc; i = i + 1) {
        int val = argv[i];
    }
    
    return 0;
}
