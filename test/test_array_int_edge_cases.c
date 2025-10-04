// Edge cases for integer arrays - Advanced patterns

int main() {
    // 1. Very large array index
    int big_array[1000];
    big_array[999] = 123;
    int val = big_array[0];
    
    // 2. Array index with expression
    int arr[10];
    int i = 5;
    arr[i + 2] = 100;
    arr[i - 1] = 200;
    arr[i * 2] = 300;
    
    // 3. Nested array indexing
    int matrix[5][5];
    int row = 2;
    int col = 3;
    matrix[row][col] = 42;
    matrix[row + 1][col - 1] = 99;
    
    // 4. Array element as array index
    int indices[3] = {0, 1, 2};
    int data[10];
    data[indices[0]] = 10;
    data[indices[1]] = 20;
    data[indices[2]] = 30;
    
    // 5. Complex array expressions
    int a[5], b[5], c[5];
    a[0] = 1;
    b[0] = 2;
    c[0] = a[0] + b[0];
    
    // 6. Array in nested loops
    int grid[10][10];
    int x, y;
    for (x = 0; x < 10; x = x + 1) {
        for (y = 0; y < 10; y = y + 1) {
            grid[x][y] = x * 10 + y;
        }
    }
    
    // 7. Array element with all operators
    int nums[10];
    nums[0] = 100;
    nums[1] = nums[0] + 50;
    nums[2] = nums[0] - 25;
    nums[3] = nums[0] * 2;
    nums[4] = nums[0] / 2;
    nums[5] = nums[0] % 7;
    nums[6] = nums[0] << 1;
    nums[7] = nums[0] >> 2;
    nums[8] = nums[0] & 0xFF;
    nums[9] = nums[0] | 0x10;
    
    // 8. Array with ternary and comma expressions  
    int test[5];
    test[0] = 10;
    test[1] = 20;
    int max = (test[0] > test[1]) ? test[0] : test[1];
    
    // 9. Array initialization with all zeros then selective assignment
    int sparse[100] = {0};
    sparse[10] = 1;
    sparse[50] = 2;
    sparse[99] = 3;
    
    // 10. Multi-level array dereferencing
    int cube[3][3][3];
    int level1 = 1;
    int level2 = 2;
    int level3 = 0;
    cube[level1][level2][level3] = 777;
    
    return 0;
}
