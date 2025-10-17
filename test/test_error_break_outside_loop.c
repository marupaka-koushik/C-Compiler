// Error test: break statement outside loop or switch
int main() {
    int x = 5;
    break;  // ERROR: break must be inside loop or switch
    return 0;
}
