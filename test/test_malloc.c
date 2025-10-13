// Comprehensive dynamic memory allocation testing

int main() {
    // Test 1: Simple malloc
    int *ptr = malloc(10);
    
    // Test 2: malloc with sizeof
    int *p1 = malloc(sizeof(int));
    
    // Test 3: malloc for array
    int *arr = malloc(5 * sizeof(int));
    
    // Test 4: Use malloced memory
    *ptr = 42;
    int val = *ptr;
    
    // Test 5: Array access on malloced memory
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    int x = arr[0];
    int y = arr[1];
    
    // Test 6: calloc (allocate and zero-initialize)
    int *p2 = calloc(5, sizeof(int));
    
    // Test 7: Use calloc memory
    p2[0] = 100;
    int z = p2[0];
    
    // Test 8: free
    free(ptr);
    
    // Test 9: free array
    free(arr);
    
    // Test 10: free calloc
    free(p2);
    
    // Test 11: malloc in expression
    int *p3 = malloc(sizeof(int) * 10);
    
    // Test 12: Multiple malloc calls
    int *m1 = malloc(4);
    int *m2 = malloc(8);
    int *m3 = malloc(16);
    
    // Test 13: malloc and assignment
    int *nums = malloc(3 * sizeof(int));
    nums[0] = 1;
    nums[1] = 2;
    nums[2] = 3;
    int sum = nums[0] + nums[1] + nums[2];
    
    // Test 14: malloc in loop
    int i;
    for (i = 0; i < 3; i = i + 1) {
        int *temp = malloc(sizeof(int));
        *temp = i;
        free(temp);
    }
    
    // Test 15: malloc with condition
    int size = 10;
    int *data;
    if (size > 5) {
        data = malloc(size * sizeof(int));
    }
    
    // Test 16: realloc
    int *original = malloc(5 * sizeof(int));
    int *resized = realloc(original, 10 * sizeof(int));
    
    // Test 17: Check malloc result
    int *check = malloc(100);
    if (check != 0) {
        *check = 999;
    }
    
    // Test 18: malloc for char array
    char *str = malloc(20 * sizeof(char));
    str[0] = 'H';
    str[1] = 'i';
    char c = str[0];
    free(str);
    
    // Test 19: Nested malloc usage
    int **matrix = malloc(3 * sizeof(int));
    
    // Test 20: malloc with arithmetic
    int n = 5;
    int *buffer = malloc((n + 1) * sizeof(int));
    
    // Test 21: calloc with variables
    int rows = 3;
    int cols = 4;
    int *grid = calloc(rows * cols, sizeof(int));
    
    // Test 22: Free multiple allocations
    free(m1);
    free(m2);
    free(m3);
    free(nums);
    free(data);
    free(resized);
    free(check);
    free(matrix);
    free(buffer);
    free(grid);
    
    return 0;
}
