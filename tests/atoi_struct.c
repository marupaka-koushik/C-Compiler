extern int* malloc(int size);

struct Config {
    int input_val;
    int mode;
};

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
    struct Config* cfg;
    char* str;
    
    // Allocate struct
    cfg = (struct Config*) malloc(8);
    
    // Create string
    str = (char*) malloc(2);
    str[0] = '5';
    str[1] = 0;
    
    // Parse and store in struct
    cfg->input_val = my_atoi(str);
    
    // Check struct value
    if (cfg->input_val != 5) return 101;
    return 0;
}
