typedef int Integer;
typedef Integer* IntPtr;

void swap_int(IntPtr a, IntPtr b) {
    Integer temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    Integer n1 = 10;
    Integer n2 = 20;
    
    swap_int(&n1, &n2);
    
    if (n1 != 20) return 1;
    if (n2 != 10) return 2;
    
    return 0;
}
