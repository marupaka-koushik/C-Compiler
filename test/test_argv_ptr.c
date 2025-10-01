// Test with single pointer argv

int main(int argc, char *argv) {
    // Test 1: Access argc
    int count = argc;
    
    // Test 2: Use argv (as pointer)
    char *arg = argv;
    
    // Test 3: Dereference argv
    char first = *argv;
    
    return 0;
}
