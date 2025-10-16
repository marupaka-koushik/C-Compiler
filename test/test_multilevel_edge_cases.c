// Edge cases for multilevel pointers

int main() {
    // Chained pointer declarations
    int x = 5;
    int *p1, **p2, ***p3;
    p1 = &x;
    p2 = &p1;
    p3 = &p2;
    
    // Mixed dereferencing and address-of
    int **pp = &p1;
    int *p_from_pp = *pp;
    int val = *p_from_pp;
    
    // Pointer to pointer arithmetic
    int arr[3] = {10, 20, 30};
    int *ptrs[3];
    ptrs[0] = &arr[0];
    ptrs[1] = &arr[1];
    ptrs[2] = &arr[2];
    
    int **pp_arr = ptrs;
    int v1 = **pp_arr;       // 10
    pp_arr = pp_arr + 1;
    int v2 = **pp_arr;       // 20
    pp_arr = pp_arr + 1;
    int v3 = **pp_arr;       // 30
    
    // Multilevel pointer with ternary operator
    int a = 1, b = 2;
    int *pa = &a, *pb = &b;
    int **pp_cond = (a > b) ? &pa : &pb;
    int result = **pp_cond;
    
    // Nested address-of operations
    int y = 100;
    int *py = &y;
    int **ppy = &py;
    int ***pppy = &ppy;
    int ****ppppy = &pppy;
    
    // Deep dereferencing
    int deep_val = ****ppppy;
    
    // Assignment chains
    int z = 50;
    int *pz = &z;
    int **ppz = &pz;
    ***(&ppz) = 75; // Modifies z
    
    return deep_val;
}
