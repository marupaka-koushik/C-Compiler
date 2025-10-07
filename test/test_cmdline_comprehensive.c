// Comprehensive command line argument test

int main(int argc, char **argv) {
    // Test 1: Access argc directly
    int argCount = argc;
    
    // Test 2: Check if arguments exist
    if (argc > 0) {
        // Program name is always argv[0]
        char *programName = argv[0];
    }
    
    // Test 3: Check for command line arguments
    if (argc > 1) {
        // First argument
        char *firstArg = argv[1];
        
        // Check for second argument
        if (argc > 2) {
            char *secondArg = argv[2];
        }
    }
    
    // Test 4: Loop through all arguments
    int i;
    for (i = 0; i < argc; i = i + 1) {
        char *currentArg = argv[i];
    }
    
    // Test 5: Use argc in calculations
    int totalArgs = argc - 1;  // Exclude program name
    int doubled = argc * 2;
    
    // Test 6: Conditional based on argument count
    if (argc == 1) {
        // No arguments provided
        int noArgs = 1;
    } else if (argc == 2) {
        // One argument
        int oneArg = 1;
    } else {
        // Multiple arguments
        int manyArgs = 1;
    }
    
    // Test 7: While loop with argc
    int k = 0;
    while (k < argc) {
        char *arg = argv[k];
        k = k + 1;
    }
    
    // Test 8: Return argc
    return argc;
}
