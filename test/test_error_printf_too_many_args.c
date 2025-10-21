// Error: printf format-argument count mismatch - too many arguments

int main() {
    int x = 10;
    int y = 20;
    int z = 30;
    
    // ERROR: One format specifier (%d) but three arguments (x, y, z)
    printf("%d", x, y, z);
    
    return 0;
}
