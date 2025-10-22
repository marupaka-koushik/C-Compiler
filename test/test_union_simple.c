// Simple union test

union Data {
    int i;
    char c;
};

int main() {
    union Data d;
    d.i = 42;
    int val = d.i;
    return 0;
}
