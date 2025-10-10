// Comprehensive test for while loops with edge cases

int main() {
    int i = 0;
    int sum = 0;
    int result = 0;
    
    // Basic while loop
    i = 0;
    while (i < 10) {
        sum = sum + i;
        i = i + 1;
    }
    
    // While loop counting down
    i = 10;
    while (i > 0) {
        sum = sum + i;
        i = i - 1;
    }
    
    // While loop with different step
    i = 0;
    while (i < 20) {
        sum = sum + i;
        i = i + 2;
    }
    
    // While loop - zero iterations (condition false from start)
    i = 10;
    while (i < 5) {
        sum = sum + 1;  // Should not execute
    }
    
    // While loop - single iteration
    i = 0;
    while (i < 1) {
        sum = sum + 100;
        i = i + 1;
    }
    
    // While loop with complex condition
    i = 0;
    while (i < 10 && i * i < 50) {
        sum = sum + i;
        i = i + 1;
    }
    
    // While loop with OR condition
    i = 0;
    while (i < 5 || sum < 100) {
        sum = sum + 1;
        i = i + 1;
        if (i > 20) {
            break;
        }
    }
    
    // While loop with break
    i = 0;
    while (i < 100) {
        if (i == 10) {
            break;
        }
        sum = sum + 1;
        i = i + 1;
    }
    
    // While loop with continue
    i = 0;
    while (i < 10) {
        i = i + 1;
        if (i == 5) {
            continue;
        }
        sum = sum + 1;
    }
    
    // While loop with multiple breaks
    i = 0;
    while (i < 20) {
        if (i == 5) {
            break;
        }
        if (i == 10) {
            break;
        }
        sum = sum + 1;
        i = i + 1;
    }
    
    // While loop with multiple continues
    i = 0;
    while (i < 10) {
        i = i + 1;
        if (i == 2) {
            continue;
        }
        if (i == 5) {
            continue;
        }
        if (i == 8) {
            continue;
        }
        sum = sum + 1;
    }
    
    // Nested while loops
    i = 0;
    while (i < 5) {
        int j = 0;
        while (j < 5) {
            sum = sum + 1;
            j = j + 1;
        }
        i = i + 1;
    }
    
    // Nested while with different conditions
    i = 0;
    while (i < 3) {
        int j = 0;
        while (j < 4) {
            sum = sum + i + j;
            j = j + 1;
        }
        i = i + 1;
    }
    
    // Triple nested while loops
    i = 0;
    while (i < 2) {
        int j = 0;
        while (j < 2) {
            int k = 0;
            while (k < 2) {
                sum = sum + 1;
                k = k + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    
    // While loop with array
    int arr[10];
    i = 0;
    while (i < 10) {
        arr[i] = i * 2;
        i = i + 1;
    }
    
    // While loop reading array
    i = 0;
    while (i < 10) {
        sum = sum + arr[i];
        i = i + 1;
    }
    
    // While loop with break in nested loop
    i = 0;
    while (i < 5) {
        int j = 0;
        while (j < 5) {
            if (j == 3) {
                break;
            }
            sum = sum + 1;
            j = j + 1;
        }
        i = i + 1;
    }
    
    // While loop with continue in nested loop
    i = 0;
    while (i < 3) {
        int j = 0;
        while (j < 5) {
            j = j + 1;
            if (j == 2) {
                continue;
            }
            sum = sum + 1;
        }
        i = i + 1;
    }
    
    // While with true condition (infinite loop with break)
    i = 0;
    while (1) {
        sum = sum + 1;
        i = i + 1;
        if (i >= 10) {
            break;
        }
    }
    
    // While with variable as condition
    i = 10;
    while (i) {
        sum = sum + 1;
        i = i - 1;
    }
    
    // While with negative condition
    i = -10;
    while (i < 0) {
        sum = sum + 1;
        i = i + 1;
    }
    
    // While with <= condition
    i = 0;
    while (i <= 10) {
        sum = sum + 1;
        i = i + 1;
    }
    
    // While with >= condition
    i = 10;
    while (i >= 0) {
        sum = sum + 1;
        i = i - 1;
    }
    
    // While with != condition
    i = 0;
    while (i != 10) {
        sum = sum + 1;
        i = i + 1;
    }
    
    // While with == condition
    i = 0;
    while (i == 0) {
        sum = sum + 100;
        i = 1;  // Exit after one iteration
    }
    
    // While without braces (single statement)
    i = 0;
    while (i < 5)
        i = i + 1;
    
    // While with empty body
    i = 0;
    while (i < 10) {
        i = i + 1;
    }
    
    // While searching for value
    i = 0;
    int found = 0;
    while (i < 10 && !found) {
        if (arr[i] == 10) {
            found = 1;
        }
        i = i + 1;
    }
    
    // While computing factorial
    int n = 5;
    int factorial = 1;
    i = 1;
    while (i <= n) {
        factorial = factorial * i;
        i = i + 1;
    }
    
    // While finding maximum
    i = 0;
    int max_val = arr[0];
    while (i < 10) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
        i = i + 1;
    }
    
    // While finding minimum
    i = 0;
    int min_val = arr[0];
    while (i < 10) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
        i = i + 1;
    }
    
    // While with accumulation
    i = 0;
    int total = 0;
    while (i < 10) {
        total = total + arr[i];
        i = i + 1;
    }
    
    // While with modulo
    i = 0;
    while (i < 20) {
        if ((i % 2) == 0) {
            sum = sum + i;
        }
        i = i + 1;
    }
    
    // While with bitwise condition
    i = 1;
    while (i < 64) {
        sum = sum + 1;
        i = i << 1;
    }
    
    // While modifying condition variable in multiple places
    i = 0;
    while (i < 20) {
        if (i == 10) {
            i = i + 5;  // Skip ahead
        }
        sum = sum + 1;
        i = i + 1;
    }
    
    // While with 2D array
    int matrix[3][3];
    i = 0;
    while (i < 3) {
        int j = 0;
        while (j < 3) {
            matrix[i][j] = i * 3 + j;
            j = j + 1;
        }
        i = i + 1;
    }
    
    // While reading 2D array
    i = 0;
    while (i < 3) {
        int j = 0;
        while (j < 3) {
            sum = sum + matrix[i][j];
            j = j + 1;
        }
        i = i + 1;
    }
    
    // While with logical NOT
    i = 0;
    while (!(i >= 10)) {
        sum = sum + 1;
        i = i + 1;
    }
    
    // While with complex arithmetic condition
    i = 0;
    while ((i * 2 + 5) < 25) {
        sum = sum + i;
        i = i + 1;
    }
    
    // While with bitwise AND in condition
    i = 15;
    while (i & 1) {
        sum = sum + i;
        i = i - 2;
    }
    
    result = sum;
    return result;
}
