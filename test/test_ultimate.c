// Ultimate test: argc, argv with multi-dimensional access, malloc, sizeof

int main(int argc, char **argv) {
    // Use argc to determine array size
    int size = argc * 10;
    
    // Allocate memory based on argc
    int *numbers = malloc(argc * sizeof(int));
    
    // Store argument count in array
    numbers[0] = argc;
    
    // Process each argument
    int i;
    for (i = 0; i < argc; i = i + 1) {
        // Get argument string
        char *arg = argv[i];
        
        // Get first character of argument
        char firstChar = argv[i][0];
        
        // Count characters in this argument
        int len = 0;
        while (argv[i][len] != '\0') {
            char c = argv[i][len];
            len = len + 1;
        }
        
        // Store length in numbers array
        if (i < argc) {
            numbers[i] = len;
        }
    }
    
    // Use double pointer dereference
    if (argc > 0) {
        char firstCharOfFirst = **argv;
    }
    
    // Free allocated memory
    free(numbers);
    
    return argc;
}
