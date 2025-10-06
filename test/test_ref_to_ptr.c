// Test reference to pointer
void refToPointer(int* &ptr) {
    int y = 10;
    ptr = &y;
}

int main() {
    int x = 5;
    int *p = &x;
    refToPointer(p);
    return 0;
}
