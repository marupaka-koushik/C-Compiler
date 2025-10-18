// Test multilevel pointers in function parameters and returns

// Function taking double pointer
void modify_ptr(int **pp) {
    int x = 99;
    *pp = &x;
}

// Function taking triple pointer
void modify_double_ptr(int ***ppp, int **new_val) {
    *ppp = new_val;
}

// Function returning double pointer
int** get_double_ptr(int *p) {
    int **pp;
    pp = &p;
    return pp;
}

// Function with array of pointers parameter
int sum_via_pointers(int **arr, int size) {
    int total = 0;
    int i = 0;
    while (i < size) {
        total = total + **arr;
        arr = arr + 1;
        i = i + 1;
    }
    return total;
}

int main() {
    int x = 10;
    int y = 20;
    int z = 30;
    
    int *px = &x;
    int *py = &y;
    int *pz = &z;
    
    // Test function with double pointer
    int **pp;
    modify_ptr(&px);
    
    // Test function with triple pointer
    int ***ppp;
    modify_double_ptr(&pp, &px);
    
    // Test function returning double pointer
    int **result = get_double_ptr(px);
    
    // Test array of pointers
    int *ptr_array[3];
    ptr_array[0] = &x;
    ptr_array[1] = &y;
    ptr_array[2] = &z;
    
    int total = sum_via_pointers(ptr_array, 3);
    
    return total;
}
