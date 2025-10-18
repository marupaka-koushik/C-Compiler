// Test multilevel pointers (double, triple, quadruple pointers)

int main() {
    // Basic multilevel pointers
    int x = 42;
    int *p = &x;        // Single pointer
    int **pp = &p;      // Double pointer (pointer to pointer)
    int ***ppp = &pp;   // Triple pointer
    int ****pppp = &ppp; // Quadruple pointer
    
    // Dereferencing multilevel pointers
    int a = *p;         // a = 42
    int b = **pp;       // b = 42
    int c = ***ppp;     // c = 42
    int d = ****pppp;   // d = 42
    
    // Assignment through multilevel pointers
    **pp = 100;         // Modifies x through double pointer
    ***ppp = 200;       // Modifies x through triple pointer
    
    // Array of pointers
    int arr[3] = {1, 2, 3};
    int *ptr_arr[3];    // Array of pointers
    ptr_arr[0] = &arr[0];
    ptr_arr[1] = &arr[1];
    ptr_arr[2] = &arr[2];
    
    int **ptr_to_arr = ptr_arr; // Pointer to array of pointers
    int val = **ptr_to_arr;     // val = 1
    
    // Pointer arithmetic with multilevel pointers
    int **pp2 = pp;
    pp2 = pp2 + 1;
    
    return 0;
}
