// Test multidimensional arrays with loops

int main() {
    int matrix[3][3];
    int i, j, k;
    
    // Initialize 2D array with nested loops
    i = 0;
    while (i < 3) {
        j = 0;
        while (j < 3) {
            matrix[i][j] = i * 3 + j;
            j = j + 1;
        }
        i = i + 1;
    }
    
    // Sum all elements
    int sum = 0;
    i = 0;
    while (i < 3) {
        j = 0;
        while (j < 3) {
            sum = sum + matrix[i][j];
            j = j + 1;
        }
        i = i + 1;
    }
    
    // 3D array with nested loops
    int cube[2][2][2];
    i = 0;
    while (i < 2) {
        j = 0;
        while (j < 2) {
            k = 0;
            while (k < 2) {
                cube[i][j][k] = i + j + k;
                k = k + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    
    int val = cube[1][1][1];
    
    return sum + val;
}
