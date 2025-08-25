// Test typedef with lexer feedback
typedef int Integer;
typedef float Real;
typedef int* IntPtr;

// Now use the typedef'd types
Integer num;
Real value;
IntPtr ptr;

Integer x;
Integer y;

int main() {
    Integer a;
    Real b;
    IntPtr p;
    
    a = 10;
    b = 3.14;
    
    return 0;
}
