// Error: Too few arguments in function call

int add(int a, int b) {
    return a + b;
}

int main() {
    // ERROR: add expects 2 arguments, but only 1 provided
    int result = add(5);
    
    return 0;
}
