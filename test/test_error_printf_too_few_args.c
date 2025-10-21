// Error: printf format-argument count mismatch - too many format specifiers

int main() {
    int x = 10;
    
    // ERROR: Two format specifiers (%d %d) but only one argument (x)
    printf("%d %d", x);
    
    return 0;
}
