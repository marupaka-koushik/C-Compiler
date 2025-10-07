int main() {
    int a = 0xFF;        // 255
    int b = 0x10;        // 16
    int c = 0xDEADBEEF;  // 3735928559
    int d = 0x0;         // 0
    int e = 0xABCD;      // 43981
    
    int result = a + b;
    result = c & 0xFF;
    result = d | e;
    
    return 0;
}
