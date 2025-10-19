// Error: Double free (freeing same memory twice)

int main() {
    int *ptr = (int *)malloc(sizeof(int) * 10);
    
    ptr[0] = 100;
    
    free(ptr);
    
    // Error: double free
    free(ptr);
    
    return 0;
}
