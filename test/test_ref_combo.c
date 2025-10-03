// Test combination of reference features

void test_basic_ref() {
    int x = 10;
    int &ref = x;
    ref = 20;
    int y = ref;
}

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

int main() {
    int p = 3;
    int q = 7;
    swap(p, q);
    
    test_basic_ref();
    
    return 0;
}
