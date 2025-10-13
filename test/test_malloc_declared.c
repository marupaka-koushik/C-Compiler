// Test with malloc declaration

// Declare malloc and free
void *malloc(int size);
void free(void *ptr);

int main() {
    // Test 1: Simple malloc call
    malloc(10);
    
    // Test 2: Simple free call
    free(0);
    
    return 0;
}
