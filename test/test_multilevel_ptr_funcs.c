// Advanced multi-level pointers with functions

void modifyValue(int ***ppp) {
    ***ppp = 999;
}

void swapPointers(int **pp1, int **pp2) {
    int *temp = *pp1;
    *pp1 = *pp2;
    *pp2 = temp;
}

int getValue(int ****pppp) {
    return ****pppp;
}

int main() {
    // Test 1: Triple pointer to function
    int x = 42;
    int *px = &x;
    int **ppx = &px;
    int ***pppx = &ppx;
    
    modifyValue(pppx);
    int result1 = x;  // Should be 999
    
    // Test 2: Swap pointers through double pointers
    int a = 10;
    int b = 20;
    int *pa = &a;
    int *pb = &b;
    
    swapPointers(&pa, &pb);
    int val1 = *pa;  // Should be 20
    int val2 = *pb;  // Should be 10
    
    // Test 3: Quadruple pointer to function
    int y = 55;
    int *py = &y;
    int **ppy = &py;
    int ***pppy = &ppy;
    int ****ppppy = &pppy;
    
    int result2 = getValue(ppppy);  // Should be 55
    
    // Test 4: Array of pointers to pointers
    int v1 = 1, v2 = 2, v3 = 3;
    int *pv1 = &v1;
    int *pv2 = &v2;
    int *pv3 = &v3;
    
    int **arr[3];
    arr[0] = &pv1;
    arr[1] = &pv2;
    arr[2] = &pv3;
    
    int val3 = **arr[0];  // Should be 1
    int val4 = **arr[1];  // Should be 2
    int val5 = **arr[2];  // Should be 3
    
    return 0;
}
