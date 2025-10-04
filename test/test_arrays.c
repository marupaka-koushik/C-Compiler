// Comprehensive test for arrays (integer and character)

int main() {
    int result = 0;
    
    // ========================================
    // TEST 1: Integer array declaration and initialization
    // ========================================
    int arr1[5] = {1, 2, 3, 4, 5};
    
    // ========================================
    // TEST 2: Integer array access
    // ========================================
    result = arr1[0];
    result = arr1[2];
    result = arr1[4];
    
    // ========================================
    // TEST 3: Integer array modification
    // ========================================
    arr1[0] = 10;
    arr1[1] = 20;
    arr1[2] = 30;
    
    // ========================================
    // TEST 4: Integer array in loop
    // ========================================
    int arr2[5];
    for (int i = 0; i < 5; i++) {
        arr2[i] = i * 2;
    }
    
    // ========================================
    // TEST 5: Reading from array in loop
    // ========================================
    int sum = 0;
    for (int j = 0; j < 5; j++) {
        sum = sum + arr1[j];
    }
    result = sum;
    
    // ========================================
    // TEST 6: Character array (string)
    // ========================================
    char str1[10] = "hello";
    
    // ========================================
    // TEST 7: Character array access
    // ========================================
    char ch1 = str1[0];
    char ch2 = str1[1];
    char ch3 = str1[4];
    
    // ========================================
    // TEST 8: Character array modification
    // ========================================
    str1[0] = 'H';
    str1[5] = '!';
    
    // ========================================
    // TEST 9: Character array in loop
    // ========================================
    char str2[5];
    for (int k = 0; k < 5; k++) {
        str2[k] = 'a';
    }
    
    // ========================================
    // TEST 10: Multi-dimensional integer array
    // ========================================
    int matrix[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    
    // ========================================
    // TEST 11: Multi-dimensional array access
    // ========================================
    result = matrix[0][0];
    result = matrix[1][1];
    result = matrix[2][2];
    
    // ========================================
    // TEST 12: Multi-dimensional array modification
    // ========================================
    matrix[0][0] = 100;
    matrix[1][1] = 200;
    matrix[2][2] = 300;
    
    // ========================================
    // TEST 13: Nested loops with 2D array
    // ========================================
    int grid[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            grid[i][j] = i + j;
        }
    }
    
    // ========================================
    // TEST 14: Array with expression as index
    // ========================================
    int idx = 2;
    result = arr1[idx];
    result = arr1[idx + 1];
    result = arr1[idx - 1];
    
    // ========================================
    // TEST 15: Array element in arithmetic
    // ========================================
    int val1 = arr1[0] + arr1[1];
    int val2 = arr1[2] * arr1[3];
    int val3 = arr1[4] - arr1[0];
    
    // ========================================
    // TEST 16: Array without initialization
    // ========================================
    int arr3[10];
    arr3[0] = 5;
    arr3[9] = 15;
    
    // ========================================
    // TEST 17: Character array without initialization
    // ========================================
    char str3[20];
    str3[0] = 'x';
    str3[19] = 'z';
    
    // ========================================
    // TEST 18: Array element as loop condition
    // ========================================
    arr1[0] = 0;
    arr1[1] = 5;
    while (arr1[0] < arr1[1]) {
        arr1[0]++;
        result++;
    }
    
    // ========================================
    // TEST 19: Array in if condition
    // ========================================
    if (arr1[0] > arr1[1]) {
        result = 100;
    } else {
        result = 200;
    }
    
    // ========================================
    // TEST 20: Incrementing array elements
    // ========================================
    arr1[0]++;
    arr1[1]--;
    ++arr1[2];
    --arr1[3];
    
    // ========================================
    // TEST 21: Compound assignment with arrays
    // ========================================
    arr1[0] += 5;
    arr1[1] -= 3;
    arr1[2] *= 2;
    arr1[3] /= 2;
    
    // ========================================
    // TEST 22: Large array
    // ========================================
    int bigArray[100];
    bigArray[0] = 1;
    bigArray[50] = 50;
    bigArray[99] = 99;
    
    // ========================================
    // TEST 23: Character array with individual chars
    // ========================================
    char chars[5] = {'a', 'b', 'c', 'd', 'e'};
    char c1 = chars[0];
    char c2 = chars[4];
    
    // ========================================
    // TEST 24: Nested array access
    // ========================================
    int lookup[3];
    lookup[0] = 1;
    lookup[1] = 3;
    lookup[2] = 4;
    result = arr1[lookup[0]];
    result = arr1[lookup[2]];
    
    // ========================================
    // TEST 25: 3D array
    // ========================================
    int cube[2][2][2] = {{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}};
    result = cube[0][0][0];
    result = cube[1][1][1];
    cube[0][1][0] = 100;
    
    // ========================================
    // TEST 26: Array with all same values
    // ========================================
    int same[5] = {7, 7, 7, 7, 7};
    result = same[0] + same[1] + same[2];
    
    // ========================================
    // TEST 27: Array element swap
    // ========================================
    int temp = arr1[0];
    arr1[0] = arr1[1];
    arr1[1] = temp;
    
    // ========================================
    // TEST 28: Character array iteration
    // ========================================
    for (int m = 0; m < 5; m++) {
        if (chars[m] == 'c') {
            result = m;
            break;
        }
    }
    
    // ========================================
    // TEST 29: Array sum calculation
    // ========================================
    int total = 0;
    for (int n = 0; n < 5; n++) {
        total += arr1[n];
    }
    result = total;
    
    // ========================================
    // TEST 30: Mixed array operations
    // ========================================
    arr1[0] = arr1[1] + arr1[2];
    arr1[3] = arr1[4] * 2;
    result = arr1[0] + arr1[3];
    
    return result;
}
