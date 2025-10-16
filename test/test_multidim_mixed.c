// Mixed multidimensional arrays and pointers

int main() {
    // 2D array
    int matrix[2][3];
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[0][2] = 3;
    matrix[1][0] = 4;
    matrix[1][1] = 5;
    matrix[1][2] = 6;
    
    // Pointer to first element
    int *p = &matrix[0][0];
    int val1 = *p;  // Should be 1
    
    // Access via calculated index (simulating pointer arithmetic)
    int val2 = matrix[0][1];
    int val3 = matrix[1][0];
    
    // 1D and 2D arrays together
    int arr1d[5];
    int arr2d[2][2];
    
    arr1d[0] = 10;
    arr1d[4] = 50;
    
    arr2d[0][0] = 100;
    arr2d[1][1] = 200;
    
    int sum = arr1d[0] + arr1d[4] + arr2d[0][0] + arr2d[1][1];
    
    // 3D array with 1D array
    int cube[2][2][2];
    cube[0][0][0] = 1;
    cube[1][1][1] = 8;
    
    int total = sum + cube[0][0][0] + cube[1][1][1];
    
    return total;
}
