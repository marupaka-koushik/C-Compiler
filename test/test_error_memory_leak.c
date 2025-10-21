// Error: Memory leak (malloc without free)

int main() {
    int *ptr = (int *)malloc(sizeof(int) * 100);
    
    ptr[0] = 10;
    ptr[50] = 20;
    
    // Error: memory leak - no free() called
    
    return 0;
}
