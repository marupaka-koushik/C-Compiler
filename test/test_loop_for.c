// Comprehensive test for for loops with edge cases

int main() {
    int i;
    int j;
    int sum = 0;
    int result = 0;
    
    // Basic for loop - counting up
    for (i = 0; i < 10; i = i + 1) {
        sum = sum + i;
    }
    
    // For loop - counting down
    for (i = 10; i > 0; i = i - 1) {
        sum = sum + i;
    }
    
    // For loop with different step
    for (i = 0; i < 20; i = i + 2) {
        sum = sum + i;
    }
    
    // For loop with step of 3
    for (i = 0; i < 30; i = i + 3) {
        sum = sum + 1;
    }
    
    // For loop starting from non-zero
    for (i = 5; i < 15; i = i + 1) {
        sum = sum + i;
    }
    
    // For loop with negative start
    for (i = -5; i < 5; i = i + 1) {
        sum = sum + i;
    }
    
    // For loop with negative step (countdown)
    for (i = 20; i >= 10; i = i - 1) {
        sum = sum + i;
    }
    
    // For loop with multiplication step
    for (i = 1; i < 100; i = i * 2) {
        sum = sum + i;
    }
    
    // For loop - zero iterations (condition false from start)
    for (i = 10; i < 5; i = i + 1) {
        sum = sum + 1;  // Should not execute
    }
    
    // For loop - single iteration
    for (i = 0; i < 1; i = i + 1) {
        sum = sum + 100;
    }
    
    // For loop - exactly 10 iterations
    int count = 0;
    for (i = 0; i < 10; i = i + 1) {
        count = count + 1;
    }
    
    // For loop with <= condition
    for (i = 0; i <= 10; i = i + 1) {
        sum = sum + 1;
    }
    
    // For loop with != condition
    for (i = 0; i != 10; i = i + 1) {
        sum = sum + 1;
    }
    
    // For loop with > condition (counting down)
    for (i = 10; i > 0; i = i - 1) {
        sum = sum + 1;
    }
    
    // For loop with >= condition
    for (i = 10; i >= 1; i = i - 1) {
        sum = sum + 1;
    }
    
    // Nested for loops - 2 levels
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            sum = sum + 1;
        }
    }
    
    // Nested for loops with different ranges
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 4; j = j + 1) {
            sum = sum + i + j;
        }
    }
    
    // Triple nested for loops
    int k;
    for (i = 0; i < 2; i = i + 1) {
        for (j = 0; j < 2; j = j + 1) {
            for (k = 0; k < 2; k = k + 1) {
                sum = sum + 1;
            }
        }
    }
    
    // For loop with array access
    int arr[10];
    for (i = 0; i < 10; i = i + 1) {
        arr[i] = i * 2;
    }
    
    // For loop reading array
    for (i = 0; i < 10; i = i + 1) {
        sum = sum + arr[i];
    }
    
    // For loop with break (early exit)
    for (i = 0; i < 100; i = i + 1) {
        if (i == 10) {
            break;
        }
        sum = sum + 1;
    }
    
    // For loop with continue (skip iteration)
    for (i = 0; i < 10; i = i + 1) {
        if (i == 5) {
            continue;
        }
        sum = sum + 1;
    }
    
    // For loop with multiple breaks
    for (i = 0; i < 20; i = i + 1) {
        if (i == 5) {
            break;
        }
        if (i == 10) {
            break;
        }
        sum = sum + 1;
    }
    
    // For loop with multiple continues
    for (i = 0; i < 10; i = i + 1) {
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
    
    // For loop with break in nested loop
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            if (j == 3) {
                break;
            }
            sum = sum + 1;
        }
    }
    
    // For loop with continue in nested loop
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            if (j == 2) {
                continue;
            }
            sum = sum + 1;
        }
    }
    
    // For loop with complex condition
    for (i = 0; i < 10 && i * i < 50; i = i + 1) {
        sum = sum + i;
    }
    
    // For loop with OR condition
    for (i = 0; i < 5 || i > 10; i = i + 1) {
        if (i > 15) {
            break;
        }
        sum = sum + 1;
    }
    
    // For loop with arithmetic in update
    for (i = 0; i < 10; i = i + 1 + 1) {
        sum = sum + 1;
    }
    
    // For loop modifying loop variable inside
    for (i = 0; i < 20; i = i + 1) {
        if (i == 10) {
            i = i + 5;  // Skip ahead
        }
        sum = sum + 1;
    }
    
    // For loop with post-increment (if supported)
    // for (i = 0; i < 10; i++) {
    //     sum = sum + 1;
    // }
    
    // For loop with empty body
    for (i = 0; i < 10; i = i + 1) {
        // Nothing
    }
    
    // For loop without braces
    for (i = 0; i < 5; i = i + 1)
        sum = sum + 1;
    
    // For loop with multiple statements in init (using pre-init)
    i = 0;
    j = 0;
    for (; i < 5; i = i + 1) {
        j = j + 1;
        sum = sum + i + j;
    }
    
    // For loop with empty init
    i = 0;
    for (; i < 10; i = i + 1) {
        sum = sum + 1;
    }
    
    // For loop with empty update
    for (i = 0; i < 10; ) {
        sum = sum + 1;
        i = i + 1;
    }
    
    // For loop with all parts present
    for (i = 0; i < 10; i = i + 1) {
        sum = sum + i;
    }
    
    // Reverse iteration
    for (i = 9; i >= 0; i = i - 1) {
        sum = sum + i;
    }
    
    // For loop with modulo condition
    for (i = 0; i < 20; i = i + 1) {
        if ((i % 2) == 0) {
            sum = sum + i;
        }
    }
    
    // For loop with bitwise operations
    for (i = 1; i < 64; i = i << 1) {
        sum = sum + 1;
    }
    
    // For loop searching for value
    int found = 0;
    for (i = 0; i < 10; i = i + 1) {
        if (arr[i] == 10) {
            found = 1;
            break;
        }
    }
    
    // For loop computing product
    int product = 1;
    for (i = 1; i <= 5; i = i + 1) {
        product = product * i;
    }
    
    // For loop with min/max finding
    int max_val = arr[0];
    for (i = 1; i < 10; i = i + 1) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
    
    int min_val = arr[0];
    for (i = 1; i < 10; i = i + 1) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
    }
    
    // For loop with accumulation
    int total = 0;
    for (i = 0; i < 10; i = i + 1) {
        total = total + arr[i];
    }
    
    // For loop with 2D array
    int matrix[3][3];
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            matrix[i][j] = i * 3 + j;
        }
    }
    
    // For loop reading 2D array
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            sum = sum + matrix[i][j];
        }
    }
    
    result = sum;
    return result;
}
