// Test mutual recursion
int isEven(int n);
int isOdd(int n);

int isEven(int n) {
    if (n == 0) {
        return 1;
    }
    return isOdd(n - 1);
}

int isOdd(int n) {
    if (n == 0) {
        return 0;
    }
    return isEven(n - 1);
}

int main() {
    int e = isEven(4);
    int o = isOdd(5);
    return 0;
}
