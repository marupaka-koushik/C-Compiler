// Comprehensive argv test - ALL features

int main(int argc, char **argv) {
    // Feature 1: Access argc
    int count = argc;
    
    // Feature 2: Single-dimensional argv access
    if (argc > 0) {
        char *progName = argv[0];
    }
    
    // Feature 3: Multi-dimensional argv access (char-by-char)
    if (argc > 0) {
        char firstChar = argv[0][0];
        char secondChar = argv[0][1];
    }
    
    // Feature 4: Loop through argument string characters
    if (argc > 1) {
        int i = 0;
        while (argv[1][i] != '\0') {
            char c = argv[1][i];
            i = i + 1;
        }
    }
    
    // Feature 5: Double pointer dereference
    if (argc > 0) {
        char first = **argv;  // First char of argv[0]
    }
    
    // Feature 6: Loop through all arguments
    int j;
    for (j = 0; j < argc; j = j + 1) {
        char *arg = argv[j];
        
        // Access individual characters
        char firstCharOfArg = argv[j][0];
    }
    
    // Feature 7: Nested loops - all args, all chars
    int k;
    for (k = 0; k < argc; k = k + 1) {
        int m = 0;
        while (argv[k][m] != '\0') {
            char ch = argv[k][m];
            m = m + 1;
        }
    }
    
    // Feature 8: Pointer arithmetic with argv
    char *secondArg;
    if (argc > 1) {
        secondArg = argv[1];
        char *ptr = secondArg;
        char val = *ptr;
    }
    
    // Feature 9: Compare argument characters
    if (argc > 1) {
        if (argv[1][0] == 'h') {
            // Help flag
            int help = 1;
        }
    }
    
    // Feature 10: Use argc and argv together
    int totalChars = 0;
    int n;
    for (n = 0; n < argc; n = n + 1) {
        int len = 0;
        while (argv[n][len] != '\0') {
            len = len + 1;
        }
        totalChars = totalChars + len;
    }
    
    return 0;
}
