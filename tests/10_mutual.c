int is_even(int n);

int is_odd(int n) {
    if (n == 0) {
        return 0;
    }
    return is_even(n - 1);
}

int is_even(int n) {
    if (n == 0) {
        return 1;
    }
    return is_odd(n - 1);
}

int main() {
    if (is_odd(11) != 1) {
        return 140;
    }
    if (is_even(10) != 1) {
        return 143;
    }
    
    return 0;
}
