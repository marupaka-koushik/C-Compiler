// Comprehensive test for command line arguments
// Tests: argc, argv, command line parsing

// Test 1: Simple argc check
int test_argc_check(int argc, char *argv[]) {
    if (argc < 2) {
        return 0;
    }
    return argc;
}

// Test 2: Print all arguments
int test_print_args(int argc, char *argv[]) {
    int i = 0;
    for (i = 0; i < argc; i = i + 1) {
        printf("Argument %d: %s\n", i, argv[i]);
    }
    return 0;
}

// Test 3: Access specific argument
int test_specific_arg(int argc, char *argv[]) {
    if (argc > 1) {
        printf("First argument: %s\n", argv[1]);
    }
    if (argc > 2) {
        printf("Second argument: %s\n", argv[2]);
    }
    return 0;
}

// Test 4: Count arguments
int test_count_args(int argc, char *argv[]) {
    int count = argc - 1;
    printf("Number of arguments: %d\n", count);
    return count;
}

// Test 5: Check for specific flag
int test_check_flag(int argc, char *argv[]) {
    int i = 0;
    for (i = 1; i < argc; i = i + 1) {
        if (argv[i][0] == '-') {
            printf("Found flag: %s\n", argv[i]);
        }
    }
    return 0;
}

// Test 6: Parse integer argument
int test_parse_int_arg(int argc, char *argv[]) {
    if (argc > 1) {
        int value = 0;
        int i = 0;
        char c = argv[1][i];
        
        while (c >= '0' && c <= '9') {
            value = value * 10 + (c - '0');
            i = i + 1;
            c = argv[1][i];
        }
        
        return value;
    }
    return 0;
}

// Test 7: Check program name
int test_program_name(int argc, char *argv[]) {
    printf("Program name: %s\n", argv[0]);
    return 0;
}

// Test 8: Loop through arguments with while
int test_while_args(int argc, char *argv[]) {
    int i = 0;
    while (i < argc) {
        printf("Arg[%d] = %s\n", i, argv[i]);
        i = i + 1;
    }
    return 0;
}

// Test 9: Loop with do-while
int test_do_while_args(int argc, char *argv[]) {
    int i = 0;
    if (argc > 0) {
        do {
            printf("%s ", argv[i]);
            i = i + 1;
        } while (i < argc);
        printf("\n");
    }
    return 0;
}

// Test 10: Conditional argument processing
int test_conditional_args(int argc, char *argv[]) {
    if (argc == 1) {
        printf("No arguments provided\n");
    } else if (argc == 2) {
        printf("One argument: %s\n", argv[1]);
    } else {
        printf("Multiple arguments: %d\n", argc - 1);
    }
    return 0;
}

// Test 11: Switch on argument count
int test_switch_argc(int argc, char *argv[]) {
    switch (argc) {
        case 1:
            printf("No arguments\n");
            break;
        case 2:
            printf("One argument\n");
            break;
        case 3:
            printf("Two arguments\n");
            break;
        default:
            printf("Many arguments\n");
            break;
    }
    return 0;
}

// Test 12: Check argument length
int test_arg_length(int argc, char *argv[]) {
    if (argc > 1) {
        int len = 0;
        int i = 0;
        
        while (argv[1][i] != '\0') {
            len = len + 1;
            i = i + 1;
        }
        
        printf("Length of first argument: %d\n", len);
        return len;
    }
    return 0;
}

// Test 13: Compare argument with string
int test_compare_arg(int argc, char *argv[]) {
    if (argc > 1) {
        char *target = "test";
        int i = 0;
        int match = 1;
        
        while (target[i] != '\0' && argv[1][i] != '\0') {
            if (target[i] != argv[1][i]) {
                match = 0;
                break;
            }
            i = i + 1;
        }
        
        if (match && target[i] == '\0' && argv[1][i] == '\0') {
            printf("Argument matches 'test'\n");
            return 1;
        }
    }
    return 0;
}

// Test 14: Nested loops with argv
int test_nested_argv(int argc, char *argv[]) {
    int i = 0;
    int j = 0;
    
    for (i = 0; i < argc; i = i + 1) {
        printf("Arg %d: ", i);
        j = 0;
        while (argv[i][j] != '\0') {
            printf("%c", argv[i][j]);
            j = j + 1;
        }
        printf("\n");
    }
    return 0;
}

// Test 15: Find argument with prefix
int test_find_prefix(int argc, char *argv[]) {
    int i = 0;
    char prefix = '-';
    
    for (i = 1; i < argc; i = i + 1) {
        if (argv[i][0] == prefix) {
            printf("Found option: %s\n", argv[i]);
        }
    }
    return 0;
}

// Test 16: Count total characters in all arguments
int test_total_chars(int argc, char *argv[]) {
    int total = 0;
    int i = 0;
    int j = 0;
    
    for (i = 0; i < argc; i = i + 1) {
        j = 0;
        while (argv[i][j] != '\0') {
            total = total + 1;
            j = j + 1;
        }
    }
    
    printf("Total characters: %d\n", total);
    return total;
}

// Test 17: Check for help flag
int test_help_flag(int argc, char *argv[]) {
    int i = 0;
    for (i = 1; i < argc; i = i + 1) {
        if (argv[i][0] == '-' && argv[i][1] == 'h') {
            printf("Help requested\n");
            return 1;
        }
    }
    return 0;
}

// Test 18: Get argument at index
char *get_arg(int argc, char *argv[], int index) {
    if (index >= 0 && index < argc) {
        return argv[index];
    }
    return 0;
}

int test_get_arg(int argc, char *argv[]) {
    char *arg = get_arg(argc, argv, 1);
    if (arg) {
        printf("Got argument: %s\n", arg);
    }
    return 0;
}

// Test 19: Process arguments in reverse
int test_reverse_args(int argc, char *argv[]) {
    int i = argc - 1;
    while (i >= 0) {
        printf("%s ", argv[i]);
        i = i - 1;
    }
    printf("\n");
    return 0;
}

// Test 20: Check if argument is numeric
int test_is_numeric(int argc, char *argv[]) {
    if (argc > 1) {
        int i = 0;
        int is_num = 1;
        
        while (argv[1][i] != '\0') {
            if (argv[1][i] < '0' || argv[1][i] > '9') {
                is_num = 0;
                break;
            }
            i = i + 1;
        }
        
        if (is_num) {
            printf("First argument is numeric\n");
            return 1;
        }
    }
    return 0;
}

// Test 21: Copy argument to buffer
int test_copy_arg(int argc, char *argv[]) {
    if (argc > 1) {
        char buffer[100];
        int i = 0;
        
        while (argv[1][i] != '\0' && i < 99) {
            buffer[i] = argv[1][i];
            i = i + 1;
        }
        buffer[i] = '\0';
        
        printf("Copied: %s\n", buffer);
    }
    return 0;
}

// Test 22: Count specific character in arguments
int test_count_char(int argc, char *argv[], char target) {
    int count = 0;
    int i = 0;
    int j = 0;
    
    for (i = 0; i < argc; i = i + 1) {
        j = 0;
        while (argv[i][j] != '\0') {
            if (argv[i][j] == target) {
                count = count + 1;
            }
            j = j + 1;
        }
    }
    
    return count;
}

// Test 23: Check for empty arguments
int test_empty_args(int argc, char *argv[]) {
    int i = 0;
    for (i = 1; i < argc; i = i + 1) {
        if (argv[i][0] == '\0') {
            printf("Empty argument at position %d\n", i);
        }
    }
    return 0;
}

// Test 24: Build string from arguments
int test_concat_args(int argc, char *argv[]) {
    int i = 0;
    int j = 0;
    
    for (i = 1; i < argc; i = i + 1) {
        j = 0;
        while (argv[i][j] != '\0') {
            printf("%c", argv[i][j]);
            j = j + 1;
        }
        if (i < argc - 1) {
            printf(" ");
        }
    }
    printf("\n");
    return 0;
}

// Test 25: Validate argument count range
int test_validate_argc(int argc, char *argv[]) {
    int min_args = 2;
    int max_args = 5;
    
    if (argc < min_args) {
        printf("Too few arguments (minimum: %d)\n", min_args);
        return -1;
    }
    
    if (argc > max_args) {
        printf("Too many arguments (maximum: %d)\n", max_args);
        return -1;
    }
    
    printf("Valid argument count: %d\n", argc);
    return 0;
}

// Main function with argc and argv
int main(int argc, char *argv[]) {
    int result = 0;
    
    // Test basic argc/argv access
    result = test_argc_check(argc, argv);
    result = test_program_name(argc, argv);
    
    // Test argument iteration
    result = test_print_args(argc, argv);
    result = test_while_args(argc, argv);
    result = test_do_while_args(argc, argv);
    
    // Test argument parsing
    result = test_specific_arg(argc, argv);
    result = test_count_args(argc, argv);
    result = test_parse_int_arg(argc, argv);
    
    // Test conditionals with argc
    result = test_conditional_args(argc, argv);
    result = test_switch_argc(argc, argv);
    
    // Test string operations on argv
    result = test_arg_length(argc, argv);
    result = test_compare_arg(argc, argv);
    result = test_is_numeric(argc, argv);
    
    // Test flags and options
    result = test_check_flag(argc, argv);
    result = test_find_prefix(argc, argv);
    result = test_help_flag(argc, argv);
    
    // Test advanced operations
    result = test_nested_argv(argc, argv);
    result = test_total_chars(argc, argv);
    result = test_reverse_args(argc, argv);
    result = test_copy_arg(argc, argv);
    result = test_count_char(argc, argv, 'a');
    result = test_empty_args(argc, argv);
    result = test_concat_args(argc, argv);
    result = test_validate_argc(argc, argv);
    
    // Test helper function
    result = test_get_arg(argc, argv);
    
    return 0;
}
