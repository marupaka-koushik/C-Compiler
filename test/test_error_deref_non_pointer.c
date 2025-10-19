// ERROR: Dereferencing non-pointer variable
int main() {
    int x = 10;
    int y = *x;  // ERROR: x is not a pointer
    return 0;
}
