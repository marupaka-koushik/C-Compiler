// ERROR: Pointer arithmetic on incompatible types
int main() {
    int x = 10;
    float f = 3.14;
    int *ptr = &x;
    float *fptr = &f;
    
    // This might work in C but could be checked for type safety
    int *result = ptr + fptr;  // ERROR: Adding two pointers
    
    return 0;
}
