// Error: Too many arguments in function call

int add(int a, int b) {
    return a + b;
}

int main() {
    // ERROR: add expects 2 arguments, but 3 provided
    int result = add(5, 10, 15);
    
    return 0;
}
