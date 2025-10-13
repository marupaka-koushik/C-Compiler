// Comprehensive multi-dimensional array test

int main() {
    // Test 1: 2D array declaration and initialization
    int matrix[3][4];
    
    // Test 2: Initialize elements
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[1][0] = 3;
    matrix[2][3] = 12;
    
    // Test 3: Read elements
    int val1 = matrix[0][0];
    int val2 = matrix[2][3];
    
    // Test 4: 2D array with initializer
    int arr2d[2][3] = {{1, 2, 3}, {4, 5, 6}};
    
    // Test 5: Access initialized array
    int a = arr2d[0][0];
    int b = arr2d[1][2];
    
    // Test 6: 3D array
    int cube[2][2][2];
    cube[0][0][0] = 100;
    cube[1][1][1] = 200;
    int c = cube[0][0][0];
    
    // Test 7: 3D array with initialization
    int arr3d[2][2][2] = {
        {{1, 2}, {3, 4}},
        {{5, 6}, {7, 8}}
    };
    
    int d = arr3d[0][1][0];
    int e = arr3d[1][1][1];
    
    // Test 8: Operations with multi-dim arrays
    int sum = matrix[0][0] + matrix[1][0];
    
    return 0;
}
