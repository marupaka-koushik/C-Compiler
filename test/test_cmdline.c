// Comprehensive command line argument testing

// Test 1: main with argc and argv
int main(int argc, char argv[]) {
    // Test 2: Access argc
    int count = argc;
    
    // Test 3: Use argc in condition
    if (argc > 1) {
        int hasArgs = 1;
    }
    
    // Test 4: Use argc in loop
    int i;
    for (i = 0; i < argc; i = i + 1) {
        int index = i;
    }
    
    // Test 5: Access argv elements
    char first = argv[0];
    
    // Test 6: argv in loop
    int j;
    for (j = 0; j < argc; j = j + 1) {
        char arg = argv[j];
    }
    
    // Test 7: argc in expression
    int total = argc + 10;
    
    // Test 8: argc comparison
    if (argc == 0) {
        int empty = 1;
    }
    
    // Test 9: argc in while loop
    int k = 0;
    while (k < argc) {
        k = k + 1;
    }
    
    return 0;
}
