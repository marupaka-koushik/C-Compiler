// Test basic multidimensional arrays

int main() {
    // 2D array declaration and initialization
    int matrix[3][4];
    
    // Initialize elements
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[0][2] = 3;
    matrix[0][3] = 4;
    
    matrix[1][0] = 5;
    matrix[1][1] = 6;
    matrix[1][2] = 7;
    matrix[1][3] = 8;
    
    matrix[2][0] = 9;
    matrix[2][1] = 10;
    matrix[2][2] = 11;
    matrix[2][3] = 12;
    
    // Access elements
    int val1 = matrix[0][0];  // 1
    int val2 = matrix[1][2];  // 7
    int val3 = matrix[2][3];  // 12
    
    // Modify elements
    matrix[1][1] = 100;
    
    // Use in expressions
    int sum = matrix[0][0] + matrix[1][1] + matrix[2][2];
    
    // 3D array
    int cube[2][3][4];
    cube[0][0][0] = 1;
    cube[1][2][3] = 99;
    
    int val4 = cube[0][0][0];
    int val5 = cube[1][2][3];
    
    return sum;
}
