// Error Test 1: Duplicate union member names
union Data {
    int x;
    float x;  // Should detect duplicate member
};

int main() {
    return 0;
}
