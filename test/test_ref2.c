void test() {
    int x = 10;
    int &ref = x;
    ref = 20;
    int y = ref;
}

int main() {
    test();
    return 0;
}
