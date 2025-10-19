// Error: Free without malloc (freeing unallocated memory)

int main() {
    int x = 10;
    int *ptr = &x;
    
    // Error: attempting to free stack variable (not from malloc)
    free(ptr);
    
    return 0;
}
