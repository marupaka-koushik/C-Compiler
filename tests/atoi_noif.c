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

int main() {
    char** args;
    char* s1;
    char* val_str;
    int result;
    
    args = (char**) malloc(8);
    s1 = (char*) malloc(2);
    s1[0] = '5';
    s1[1] = 0;
    args[1] = s1;
    
    val_str = args[1];
    result = my_atoi(val_str);
    
    if (result != 5) return 107;
    return 0;
}
