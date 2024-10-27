int g_val = 0;

int sum_four(int a, int b, int c, int d) {
    return a + b + c + d;
}

void register_destroyer() {
    int a = 1, b = 2, c = 3, d = 4;
    int e = 5, f = 6, g = 7, h = 8;
    g_val = a + b + c + d + e + f + g + h;
}

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int is_even(int n);
int is_odd(int n) {
    if (n == 0) return 0;
    return is_even(n - 1);
}
int is_even(int n) {
    if (n == 0) return 1;
    return is_odd(n - 1);
}

int main() {
    int result = sum_four(1, 2, 3, 4);
    if (result != 10) return 96;
    
    result = sum_four(10+20, 10-20, 10+20, 10-20);
    if (result != 40) return 104;
    
    int x = 999, y = 777;
    register_destroyer();
    if (g_val != 36) return 116;
    if (x != 999) return 121;
    if (y != 777) return 124;
    
    result = factorial(5);
    if (result != 120) return 133;
    
    if (is_odd(5) != 1) return 140;
    if (is_even(4) != 1) return 143;
    
    return 0;
}
