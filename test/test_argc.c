// Test with just argc

int main(int argc) {
    // Test 1: Access argc
    int count = argc;
    
    // Test 2: Use argc in condition
    if (argc > 1) {
        int hasArgs = 1;
    }
    
    // Test 3: Use argc in loop
    int i;
    for (i = 0; i < argc; i = i + 1) {
        int index = i;
    }
    
    // Test 4: argc in expression
    int total = argc + 10;
    int half = argc / 2;
    
    // Test 5: argc comparison
    if (argc == 0) {
        int empty = 1;
    } else {
        int notEmpty = 1;
    }
    
    // Test 6: argc in while loop
    int k = 0;
    while (k < argc) {
        k = k + 1;
    }
    
    // Test 7: argc as return value
    return argc;
}
