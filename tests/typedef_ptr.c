typedef int Number;
typedef Number* IntPtr;

int main() {
    Number n1 = 10;
    IntPtr p = &n1;
    *p = 50;
    
    if (n1 != 50) {
        return 1;
    }
    
    return 0;
}
