// Edge cases for multidimensional arrays

int main() {
    // Different sized dimensions
    int rect[2][5];
    rect[0][0] = 1;
    rect[0][4] = 5;
    rect[1][0] = 6;
    rect[1][4] = 10;
    
    // 4D array
    int hyper[2][2][2][2];
    hyper[0][0][0][0] = 1;
    hyper[1][1][1][1] = 16;
    
    // Array of arrays with different dimensions
    int arr3d[3][2][4];
    arr3d[0][0][0] = 1;
    arr3d[2][1][3] = 99;
    
    // Using variables as indices
    int i = 1;
    int j = 0;
    int k = 2;
    int l = 1;
    
    int val1 = rect[i][j];
    int val2 = hyper[i][i][i][i];
    int val3 = arr3d[k][l][k];
    
    // Complex expressions as indices
    int val4 = rect[i + 0][j + 4];
    int val5 = arr3d[i + 1][j + 1][k + 1];
    
    // Multidimensional with expressions
    rect[0][0] = rect[0][0] + rect[1][4];
    
    // Chain access
    int x = hyper[0][0][0][0];
    int y = hyper[1][1][1][1];
    int sum = x + y;
    
    return sum;
}
