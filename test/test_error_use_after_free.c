// Error: Using allocated memory after free (use-after-free)

int main() {
    int *ptr = (int *)malloc(sizeof(int) * 5);
    
    ptr[0] = 10;
    
    free(ptr);
    
    // Error: accessing freed memory
    ptr[1] = 20;
    
    return 0;
}
