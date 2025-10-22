// Test pointer-based reference passing
void increment(int* ptr) {
    *ptr = *ptr + 1;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int x = 10;
    int y = 20;
    
    // Pass by reference using pointers
    increment(&x);
    swap(&x, &y);
    
    return x + y;
}
