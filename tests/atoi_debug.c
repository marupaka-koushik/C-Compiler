extern int* malloc(int size);

int my_atoi(char* s) {
    int res = 0;
    while (*s != 0) {
        int digit = *s - 48;
        if (digit < 0) return 0;
        if (digit > 9) return 0;
        res = res * 10 + digit;
        s = s + 1;
    }
    return res;
}

int main(int argc, char** argv) {
    char** args_to_use;
    int args_count;
    int result;
    
    if (argc <= 1) {
        // Mock mode
        args_count = 3;
        args_to_use = (char**) malloc(12);
        
        // Create string "5"
        char* s1 = (char*) malloc(2);
        s1[0] = '5';
        s1[1] = 0;
        args_to_use[1] = s1;
    } else {
        args_to_use = argv;
        args_count = argc;
    }
    
    if (args_count > 1) {
        char* val_str = args_to_use[1];
        result = my_atoi(val_str);
    }
    
    if (result != 5) return 107;
    return 0;
}
