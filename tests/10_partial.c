int g_val = 0;

void register_destroyer() {
    int a = 1, b = 2, c = 3, d = 4;
    int e = 5, f = 6, g = 7, h = 8;
    g_val = a + b + c + d + e + f + g + h;
}

int main() {
    int x = 999;
    int y = 777;
    
    register_destroyer();
    
    if (g_val != 36) {
        return 116;
    }
    
    if (x != 999) {
        return 121;
    }
    
    if (y != 777) {
        return 124;
    }
    
    return 0;
}
