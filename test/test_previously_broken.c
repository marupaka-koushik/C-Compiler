// Demonstration: Previously broken, now working!

int main(int argc, char **argv) {
    // PREVIOUSLY BROKEN #1: Multi-dimensional access argv[i][j]
    if (argc > 0) {
        char c1 = argv[0][0];  // ✅ NOW WORKS!
        char c2 = argv[0][1];  // ✅ NOW WORKS!
    }
    
    // PREVIOUSLY BROKEN #2: String traversal
    if (argc > 1) {
        int i = 0;
        while (argv[1][i] != '\0') {  // ✅ NOW WORKS!
            char ch = argv[1][i];      // ✅ NOW WORKS!
            i = i + 1;
        }
    }
    
    // PREVIOUSLY BROKEN #3: Double pointer dereference
    if (argc > 0) {
        char first = **argv;  // ✅ NOW WORKS!
    }
    
    // PREVIOUSLY BROKEN #4: Nested loops through all arguments
    int j;
    for (j = 0; j < argc; j = j + 1) {
        int k = 0;
        while (argv[j][k] != '\0') {  // ✅ NOW WORKS!
            char c = argv[j][k];       // ✅ NOW WORKS!
            k = k + 1;
        }
    }
    
    return 0;
}
