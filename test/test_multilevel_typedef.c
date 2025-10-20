// Test multilevel pointers with typedef

typedef int* IntPtr;
typedef IntPtr* IntPtrPtr;
typedef IntPtrPtr* IntPtrPtrPtr;

typedef char** StringArray;
typedef int*** TripleIntPtr;

int main() {
    // Using typedef for single pointer
    int x = 42;
    IntPtr p1 = &x;
    int val1 = *p1;
    
    // Using typedef for double pointer
    IntPtr *p2 = &p1;
    IntPtrPtr p3 = &p1;
    int val2 = **p2;
    int val3 = **p3;
    
    // Using typedef for triple pointer
    IntPtrPtr *p4 = &p3;
    IntPtrPtrPtr p5 = &p3;
    int val4 = ***p4;
    int val5 = ***p5;
    
    // Complex typedef usage
    TripleIntPtr tp;
    IntPtr ip = &x;
    IntPtrPtr ipp = &ip;
    tp = &ipp;
    int result = ***tp;
    
    // Array with typedef
    IntPtr arr[5];
    int a = 1, b = 2, c = 3;
    arr[0] = &a;
    arr[1] = &b;
    arr[2] = &c;
    
    IntPtrPtr ptr_to_arr = arr;
    int first = **ptr_to_arr;
    
    return result;
}
