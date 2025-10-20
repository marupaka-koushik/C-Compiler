// Test 2: Typedef with pointers
typedef int* IntPtr;
typedef char** CharPtrPtr;

int main() {
    int x = 42;
    IntPtr p = &x;
    int val = *p;
    
    char c = 'A';
    char *pc = &c;
    CharPtrPtr ppc = &pc;
    char result = **ppc;
    
    return 0;
}
