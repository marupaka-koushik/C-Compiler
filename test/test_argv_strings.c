// Test what currently doesn't work with argv

int main(int argc, char **argv) {
    // Test 1: Access individual characters in argument string
    if (argc > 0) {
        char first_char = argv[0][0];  // First character of program name
    }
    
    // Test 2: Loop through characters in an argument
    if (argc > 1) {
        int i = 0;
        while (argv[1][i] != '\0') {
            char c = argv[1][i];
            i = i + 1;
        }
    }
    
    // Test 3: Double pointer dereference
    char first = **argv;  // First char of first arg
    
    return 0;
}
