// ERROR: Array index must be constant in declaration
int main() {
    int n = 5;
    int arr[n];  // ERROR: Array size must be constant, not variable (VLA not supported)
    return 0;
}
