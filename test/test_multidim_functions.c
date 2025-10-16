// Test multidimensional arrays with functions

// Function taking 2D array as parameter
int sum_matrix(int arr[3][3]) {
    int sum = 0;
    int i = 0;
    int j;
    
    while (i < 3) {
        j = 0;
        while (j < 3) {
            sum = sum + arr[i][j];
            j = j + 1;
        }
        i = i + 1;
    }
    return sum;
}

// Function to initialize 2D array
void init_matrix(int mat[2][2]) {
    mat[0][0] = 1;
    mat[0][1] = 2;
    mat[1][0] = 3;
    mat[1][1] = 4;
}

// Function with 3D array parameter
int get_element_3d(int arr[2][2][2], int i, int j, int k) {
    return arr[i][j][k];
}

int main() {
    int matrix[3][3];
    int i = 0;
    int j;
    
    // Initialize
    while (i < 3) {
        j = 0;
        while (j < 3) {
            matrix[i][j] = i + j;
            j = j + 1;
        }
        i = i + 1;
    }
    
    // Call function
    int total = sum_matrix(matrix);
    
    // Test with smaller array
    int small[2][2];
    init_matrix(small);
    int val = small[1][1];
    
    // Test 3D array
    int cube[2][2][2];
    cube[1][1][1] = 99;
    int elem = get_element_3d(cube, 1, 1, 1);
    
    return total + val + elem;
}
