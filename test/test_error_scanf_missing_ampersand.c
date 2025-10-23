// Error: scanf missing address-of operator

int main() {
    int x;
    
    // ERROR: Should be &x, not x (scanf needs pointer/address)
    scanf("%d", x);
    
    return 0;
}
