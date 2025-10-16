// Test complex multilevel pointer scenarios

// Function returning triple pointer
int*** create_triple_ptr() {
    int x = 100;
    int *p = &x;
    int **pp = &p;
    int ***ppp = &pp;
    return ppp;
}

// Function with multilevel pointer parameters
void swap_pointers(int **p1, int **p2) {
    int *temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

// Function modifying through triple pointer
void modify_via_triple(int ***ppp, int new_val) {
    ***ppp = new_val;
}

int main() {
    // Test 1: Array of pointer arrays
    int a = 1, b = 2, c = 3, d = 4;
    int *row1[2];
    int *row2[2];
    row1[0] = &a;
    row1[1] = &b;
    row2[0] = &c;
    row2[1] = &d;
    
    int **matrix[2];
    matrix[0] = row1;
    matrix[1] = row2;
    
    int ***ptr_to_matrix = matrix;
    int val1 = ***ptr_to_matrix;  // Should be 1
    
    // Test 2: Swap pointers
    int x = 10, y = 20;
    int *px = &x;
    int *py = &y;
    swap_pointers(&px, &py);
    int val2 = *px;  // Should be 20
    
    // Test 3: Modify via triple pointer
    int z = 50;
    int *pz = &z;
    int **ppz = &pz;
    int ***pppz = &ppz;
    modify_via_triple(pppz, 99);
    int val3 = z;  // Should be 99
    
    // Test 4: Pointer to function pointer (if supported)
    // Skip for now - may not be supported
    
    // Test 5: Conditional with multilevel pointers
    int m = 5, n = 15;
    int *pm = &m;
    int *pn = &n;
    int **pp = (m > n) ? &pm : &pn;
    int val4 = **pp;  // Should be 15
    
    // Test 6: Loop with multilevel pointers
    int nums[3] = {100, 200, 300};
    int *ptrs[3];
    ptrs[0] = &nums[0];
    ptrs[1] = &nums[1];
    ptrs[2] = &nums[2];
    
    int **p_iter = ptrs;
    int sum = 0;
    int i = 0;
    while (i < 3) {
        sum = sum + **p_iter;
        p_iter = p_iter + 1;
        i = i + 1;
    }
    
    return sum;
}
