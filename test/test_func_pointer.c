// Test pointer argument only

int getValue(int *ptr) {
    return *ptr;
}

int main() {
    int num = 42;
    int *ptr = &num;
    int r1 = getValue(ptr);
    
    return 0;
}
