// Comprehensive test for dynamic memory allocation
// Tests: malloc, calloc, realloc, free (without explicit casts)

// Test 1: Simple malloc and free
int test_simple_malloc() {
    int *ptr = malloc(sizeof(int) * 5);
    *ptr = 10;
    ptr[1] = 20;
    ptr[2] = 30;
    free(ptr);
    return 0;
}

// Test 2: Malloc with sizeof operator
int test_malloc_sizeof() {
    int *numbers = malloc(sizeof(int) * 10);
    char *characters = malloc(sizeof(char) * 100);
    
    int i = 0;
    for (i = 0; i < 10; i = i + 1) {
        numbers[i] = i * 2;
    }
    
    free(numbers);
    free(characters);
    return 0;
}

// Test 3: Calloc (zero-initialized allocation)
int test_calloc() {
    int *arr = calloc(10, sizeof(int));
    
    int i = 0;
    for (i = 0; i < 10; i = i + 1) {
        arr[i] = i;
    }
    
    free(arr);
    return 0;
}

// Test 4: Realloc (resize allocation)
int test_realloc() {
    int *ptr = malloc(sizeof(int) * 5);
    
    ptr[0] = 1;
    ptr[1] = 2;
    ptr[2] = 3;
    
    // Resize to 10 elements
    ptr = realloc(ptr, sizeof(int) * 10);
    
    ptr[5] = 6;
    ptr[9] = 10;
    
    free(ptr);
    return 0;
}

// Test 5: Multiple allocations
int test_multiple_allocations() {
    int *ptr1 = malloc(sizeof(int) * 5);
    int *ptr2 = malloc(sizeof(int) * 10);
    int *ptr3 = malloc(sizeof(int) * 15);
    
    *ptr1 = 100;
    *ptr2 = 200;
    *ptr3 = 300;
    
    free(ptr1);
    free(ptr2);
    free(ptr3);
    return 0;
}

// Test 6: Allocate struct dynamically
struct Point {
    int x;
    int y;
};

int test_struct_allocation() {
    struct Point *p = malloc(sizeof(struct Point));
    
    p->x = 10;
    p->y = 20;
    
    free(p);
    return 0;
}

// Test 7: Array of structs
int test_struct_array_allocation() {
    struct Point *points = malloc(sizeof(struct Point) * 5);
    
    int i = 0;
    for (i = 0; i < 5; i = i + 1) {
        points[i].x = i;
        points[i].y = i * 2;
    }
    
    free(points);
    return 0;
}

// Test 8: Calloc with different sizes
int test_calloc_sizes() {
    int *ints = calloc(5, sizeof(int));
    char *chars = calloc(100, sizeof(char));
    
    ints[0] = 10;
    chars[0] = 'A';
    
    free(ints);
    free(chars);
    return 0;
}

// Test 9: Realloc to larger size
int test_realloc_larger() {
    int *arr = malloc(sizeof(int) * 5);
    
    int i = 0;
    for (i = 0; i < 5; i = i + 1) {
        arr[i] = i + 1;
    }
    
    arr = realloc(arr, sizeof(int) * 20);
    
    for (i = 5; i < 20; i = i + 1) {
        arr[i] = i + 1;
    }
    
    free(arr);
    return 0;
}

// Test 10: Realloc to smaller size
int test_realloc_smaller() {
    int *arr = malloc(sizeof(int) * 20);
    
    int i = 0;
    for (i = 0; i < 20; i = i + 1) {
        arr[i] = i;
    }
    
    arr = realloc(arr, sizeof(int) * 5);
    
    free(arr);
    return 0;
}

// Test 11: Allocation in loop
int test_allocation_in_loop() {
    int i = 0;
    int *temp = 0;
    
    for (i = 0; i < 5; i = i + 1) {
        temp = malloc(sizeof(int));
        *temp = i * 10;
        free(temp);
    }
    
    return 0;
}

// Test 12: Allocation with conditional
int test_conditional_allocation(int size) {
    int *arr = 0;
    
    if (size > 0) {
        arr = malloc(sizeof(int) * size);
        arr[0] = 100;
        free(arr);
    }
    
    return 0;
}

// Test 13: Nested allocations
int test_nested_allocations() {
    int *outer = malloc(sizeof(int) * 10);
    
    if (outer) {
        int *inner = malloc(sizeof(int) * 5);
        
        *outer = 1;
        *inner = 2;
        
        free(inner);
    }
    
    free(outer);
    return 0;
}

// Test 14: String allocation
int test_string_allocation() {
    char *str = malloc(sizeof(char) * 50);
    
    str[0] = 'H';
    str[1] = 'e';
    str[2] = 'l';
    str[3] = 'l';
    str[4] = 'o';
    str[5] = '\0';
    
    free(str);
    return 0;
}

// Test 15: Allocation with sizeof expression
int test_sizeof_expressions() {
    int n = 10;
    int *arr1 = malloc(n * sizeof(int));
    int *arr2 = malloc(sizeof(int) * n);
    
    arr1[0] = 1;
    arr2[0] = 2;
    
    free(arr1);
    free(arr2);
    return 0;
}

// Test 16: Calloc vs malloc
int test_calloc_vs_malloc() {
    int *with_malloc = malloc(sizeof(int) * 10);
    int *with_calloc = calloc(10, sizeof(int));
    
    int i = 0;
    for (i = 0; i < 10; i = i + 1) {
        with_malloc[i] = 0;
        with_calloc[i] = i;
    }
    
    free(with_malloc);
    free(with_calloc);
    return 0;
}

// Test 17: Allocation for function return
int *allocate_array(int size) {
    int *arr = malloc(sizeof(int) * size);
    
    int i = 0;
    for (i = 0; i < size; i = i + 1) {
        arr[i] = i * i;
    }
    
    return arr;
}

int test_function_allocation() {
    int *result = allocate_array(10);
    int value = result[5];
    free(result);
    return value;
}

// Test 18: Allocation with while loop
int test_allocation_while() {
    int count = 0;
    int *arr = malloc(sizeof(int) * 10);
    
    while (count < 10) {
        arr[count] = count;
        count = count + 1;
    }
    
    free(arr);
    return 0;
}

// Test 19: Multiple reallocs
int test_multiple_reallocs() {
    int *arr = malloc(sizeof(int) * 5);
    
    arr[0] = 1;
    
    arr = realloc(arr, sizeof(int) * 10);
    arr[9] = 10;
    
    arr = realloc(arr, sizeof(int) * 20);
    arr[19] = 20;
    
    arr = realloc(arr, sizeof(int) * 15);
    
    free(arr);
    return 0;
}

// Test 20: Mixed allocation types
int test_mixed_allocations() {
    int *arr1 = malloc(sizeof(int) * 5);
    int *arr2 = calloc(5, sizeof(int));
    
    arr1[0] = 1;
    arr2[0] = 2;
    
    arr1 = realloc(arr1, sizeof(int) * 10);
    
    free(arr1);
    free(arr2);
    
    return 0;
}

// Test 21: Allocation with do-while
int test_do_while_allocation() {
    int *arr = malloc(sizeof(int) * 5);
    int i = 0;
    
    do {
        arr[i] = i * 2;
        i = i + 1;
    } while (i < 5);
    
    free(arr);
    return 0;
}

// Test 22: Allocation in switch
int test_allocation_switch(int choice) {
    int *ptr = 0;
    
    switch (choice) {
        case 1:
            ptr = malloc(sizeof(int) * 10);
            ptr[0] = 10;
            break;
        case 2:
            ptr = calloc(10, sizeof(int));
            ptr[0] = 20;
            break;
        default:
            ptr = malloc(sizeof(int));
            *ptr = 30;
            break;
    }
    
    free(ptr);
    return 0;
}

// Test 23: Sequential allocations and frees
int test_sequential() {
    int *p1 = malloc(sizeof(int));
    *p1 = 1;
    free(p1);
    
    int *p2 = malloc(sizeof(int) * 2);
    p2[0] = 2;
    free(p2);
    
    int *p3 = calloc(3, sizeof(int));
    p3[0] = 3;
    free(p3);
    
    return 0;
}

// Test 24: Allocation with complex expressions
int test_complex_expr() {
    int a = 5;
    int b = 10;
    int *arr = malloc(sizeof(int) * (a + b));
    
    arr[0] = a;
    arr[14] = b;
    
    free(arr);
    return 0;
}

// Test 25: Nested struct allocation
struct Container {
    int value;
    int count;
};

int test_nested_struct() {
    struct Container *c = malloc(sizeof(struct Container));
    
    c->value = 100;
    c->count = 5;
    
    free(c);
    return 0;
}

int main() {
    int result = 0;
    
    // Test basic allocations
    result = test_simple_malloc();
    result = test_malloc_sizeof();
    result = test_calloc();
    result = test_realloc();
    
    // Test multiple allocations
    result = test_multiple_allocations();
    
    // Test struct allocations
    result = test_struct_allocation();
    result = test_struct_array_allocation();
    
    // Test patterns
    result = test_calloc_sizes();
    result = test_realloc_larger();
    result = test_realloc_smaller();
    
    // Test with control structures
    result = test_allocation_in_loop();
    result = test_conditional_allocation(10);
    result = test_nested_allocations();
    
    // Test string and expressions
    result = test_string_allocation();
    result = test_sizeof_expressions();
    result = test_calloc_vs_malloc();
    
    // Test function allocations
    result = test_function_allocation();
    
    // Test with various control structures
    result = test_allocation_while();
    result = test_multiple_reallocs();
    result = test_mixed_allocations();
    result = test_do_while_allocation();
    result = test_allocation_switch(1);
    result = test_sequential();
    result = test_complex_expr();
    result = test_nested_struct();
    
    return 0;
}
