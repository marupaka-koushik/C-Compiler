// Test reference return

int& getMax(int &a, int &b) {
    if (a > b) {
        return a;
    }
    return b;
}

int main() {
    int x = 5;
    int y = 10;
    int &max = getMax(x, y);
    max = 99;
    return 0;
}
