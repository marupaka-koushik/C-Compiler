extern int* malloc(int size);

int my_atoi(char* s) {
    int res = 0;
    int sign = 1;
    
    if (*s == '-') {
        sign = -1;
        s = s + 1;
    }
    
    while (*s != 0) {
        int digit = *s - 48;
        if (digit < 0) return 0;
        if (digit > 9) return 0;
        res = res * 10 + digit;
        s = s + 1;
    }
    return res * sign;
}

int main() {
    char* str;
    int val;
    
    str = (char*) malloc(2);
    str[0] = '5';
    str[1] = 0;
    
    val = my_atoi(str);
    
    if (val != 5) return 101;
    return 0;
}
