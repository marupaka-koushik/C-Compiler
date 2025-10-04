// Comprehensive test for integer arrays - VALID cases

int main() {
    // 1. Basic array declaration and initialization
    int arr1[5];
    int arr2[3] = {1, 2, 3};
    int arr3[] = {10, 20, 30, 40};
    
    // 2. Array element access and assignment
    arr1[0] = 100;
    arr1[1] = 200;
    arr1[2] = arr1[0] + arr1[1];
    int x = arr2[1];
    
    // 3. Array in expressions
    int sum = arr2[0] + arr2[1] + arr2[2];
    int product = arr1[0] * 2;
    
    // 4. Array with loops
    int i;
    for (i = 0; i < 5; i = i + 1) {
        arr1[i] = i * 10;
    }
    
    // 5. Nested array access in loops
    int total = 0;
    for (i = 0; i < 3; i = i + 1) {
        total = total + arr2[i];
    }
    
    // 6. Array element in conditional
    if (arr1[0] > arr1[1]) {
        x = arr1[0];
    } else {
        x = arr1[1];
    }
    
    // 7. Array element in switch
    int choice = arr2[0];
    switch(choice) {
        case 1:
            x = 10;
            break;
        case 2:
            x = 20;
            break;
        default:
            x = 0;
            break;
    }
    
    // 8. Multi-dimensional arrays
    int matrix[3][3];
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[1][0] = 3;
    int val = matrix[0][0];
    
    // 9. 3D arrays
    int cube[2][2][2];
    cube[0][0][0] = 1;
    cube[1][1][1] = 8;
    
    // 10. Array initialization with expressions
    int arr4[5] = {1 + 1, 2 * 2, 3 + 3, 4 * 4, 5 + 5};
    
    // 11. Zero initialization
    int zeros[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    // 12. Partial initialization
    int partial[5] = {1, 2};  // Rest should be 0
    
    // 13. Array element increment/decrement
    arr1[0]++;
    arr1[1]--;
    ++arr1[2];
    --arr1[3];
    
    // 14. Compound assignment with array elements
    arr1[0] += 10;
    arr1[1] -= 5;
    arr1[2] *= 2;
    arr1[3] /= 2;
    arr1[4] %= 3;
    
    // 15. Array element in ternary operator
    x = (arr1[0] > arr1[1]) ? arr1[0] : arr1[1];
    
    // 16. Array with negative values
    int negatives[3] = {-1, -2, -3};
    
    // 17. Large array
    int large[100];
    large[99] = 9999;
    
    // 18. Array element in function-like operations
    int temp = arr1[0];
    arr1[0] = arr1[1];
    arr1[1] = temp;  // Swap
    
    // 19. Bitwise operations on array elements
    arr1[0] = arr1[0] & 0xFF;
    arr1[1] = arr1[1] | 0x10;
    arr1[2] = arr1[2] ^ 0xAA;
    arr1[3] = arr1[3] << 2;
    arr1[4] = arr1[4] >> 1;
    
    // 20. Logical operations with array elements
    int flag = (arr1[0] > 0) && (arr1[1] < 100);
    flag = (arr1[0] == 0) || (arr1[1] != 0);
    
    return 0;
}
