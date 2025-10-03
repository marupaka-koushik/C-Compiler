// Test reference parameters
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void increment(int &n) {
    n = n + 1;
}

int main() {
    int x = 5;
    int y = 10;
    swap(x, y);
    
    int z = 0;
    increment(z);
    
    return 0;
}
