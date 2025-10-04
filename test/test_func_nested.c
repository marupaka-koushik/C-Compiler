// Test function calling another function

int add(int x, int y) {
    return x + y;
}

int addAndDouble(int x, int y) {
    int sum = add(x, y);
    return sum * 2;
}

int main() {
    int r = addAndDouble(5, 10);
    return 0;
}
