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
    char* str;
    int val;
    
    // Allocate array of pointers
    args = (char**) malloc(8);  // 2 pointers
    
    // Create string
    str = (char*) malloc(2);
    str[0] = '5';
    str[1] = 0;
    
    // Store in array
    args[0] = str;
    
    // Load from array and parse
    val = my_atoi(args[0]);
    
    if (val != 5) return 101;
    return 0;
}
