// Test multidimensional array initialization patterns

int main() {
    // Initialize all elements systematically
    int matrix[2][3];
    
    // Row 0
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[0][2] = 3;
    
    // Row 1
    matrix[1][0] = 4;
    matrix[1][1] = 5;
    matrix[1][2] = 6;
    
    // Access and compute
    int sum_row0 = matrix[0][0] + matrix[0][1] + matrix[0][2];
    int sum_row1 = matrix[1][0] + matrix[1][1] + matrix[1][2];
    int total = sum_row0 + sum_row1;
    
    // 3D initialization
    int cube[2][2][2];
    
    // Layer 0
    cube[0][0][0] = 1;
    cube[0][0][1] = 2;
    cube[0][1][0] = 3;
    cube[0][1][1] = 4;
    
    // Layer 1
    cube[1][0][0] = 5;
    cube[1][0][1] = 6;
    cube[1][1][0] = 7;
    cube[1][1][1] = 8;
    
    int sum_cube = cube[0][0][0] + cube[0][0][1] + 
                   cube[0][1][0] + cube[0][1][1] +
                   cube[1][0][0] + cube[1][0][1] + 
                   cube[1][1][0] + cube[1][1][1];
    
    return total + sum_cube;
}
