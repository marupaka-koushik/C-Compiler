// Mixed loop types and complex loop scenarios

int main() {
    int sum = 0;
    int i;
    int j;
    int k;
    
    // For loop inside while loop
    i = 0;
    while (i < 3) {
        for (j = 0; j < 4; j = j + 1) {
            sum = sum + 1;
        }
        i = i + 1;
    }
    
    // While loop inside for loop
    for (i = 0; i < 3; i = i + 1) {
        j = 0;
        while (j < 4) {
            sum = sum + 1;
            j = j + 1;
        }
    }
    
    // Do-while inside for loop
    for (i = 0; i < 3; i = i + 1) {
        j = 0;
        do {
            sum = sum + 1;
            j = j + 1;
        } while (j < 4);
    }
    
    // For loop inside do-while loop
    i = 0;
    do {
        for (j = 0; j < 4; j = j + 1) {
            sum = sum + 1;
        }
        i = i + 1;
    } while (i < 3);
    
    // While loop inside do-while loop
    i = 0;
    do {
        j = 0;
        while (j < 4) {
            sum = sum + 1;
            j = j + 1;
        }
        i = i + 1;
    } while (i < 3);
    
    // Do-while inside while loop
    i = 0;
    while (i < 3) {
        j = 0;
        do {
            sum = sum + 1;
            j = j + 1;
        } while (j < 4);
        i = i + 1;
    }
    
    // Triple nesting: for-while-do
    for (i = 0; i < 2; i = i + 1) {
        j = 0;
        while (j < 2) {
            k = 0;
            do {
                sum = sum + 1;
                k = k + 1;
            } while (k < 2);
            j = j + 1;
        }
    }
    
    // Triple nesting: while-for-do
    i = 0;
    while (i < 2) {
        for (j = 0; j < 2; j = j + 1) {
            k = 0;
            do {
                sum = sum + 1;
                k = k + 1;
            } while (k < 2);
        }
        i = i + 1;
    }
    
    // Triple nesting: do-while-for
    i = 0;
    do {
        j = 0;
        while (j < 2) {
            for (k = 0; k < 2; k = k + 1) {
                sum = sum + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    } while (i < 2);
    
    // Break from inner for loop
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            if (j == 3) {
                break;  // Breaks inner for loop only
            }
            sum = sum + 1;
        }
    }
    
    // Break from inner while loop
    i = 0;
    while (i < 5) {
        j = 0;
        while (j < 5) {
            if (j == 3) {
                break;  // Breaks inner while loop only
            }
            sum = sum + 1;
            j = j + 1;
        }
        i = i + 1;
    }
    
    // Break from inner do-while loop
    i = 0;
    do {
        j = 0;
        do {
            if (j == 3) {
                break;  // Breaks inner do-while loop only
            }
            sum = sum + 1;
            j = j + 1;
        } while (j < 5);
        i = i + 1;
    } while (i < 5);
    
    // Continue in mixed nested loops
    for (i = 0; i < 3; i = i + 1) {
        j = 0;
        while (j < 5) {
            j = j + 1;
            if (j == 2) {
                continue;  // Continues inner while loop
            }
            sum = sum + 1;
        }
    }
    
    // Multiple breaks at different levels
    i = 0;
    while (i < 10) {
        if (i == 8) {
            break;  // Outer break
        }
        for (j = 0; j < 10; j = j + 1) {
            if (j == 5) {
                break;  // Inner break
            }
            sum = sum + 1;
        }
        i = i + 1;
    }
    
    // Infinite loops with breaks
    i = 0;
    while (1) {
        j = 0;
        while (1) {
            sum = sum + 1;
            j = j + 1;
            if (j >= 5) {
                break;
            }
        }
        i = i + 1;
        if (i >= 3) {
            break;
        }
    }
    
    // Loop with array operations
    int arr[10];
    for (i = 0; i < 10; i = i + 1) {
        arr[i] = 0;
    }
    
    // Fill array with nested loops
    i = 0;
    while (i < 10) {
        j = 0;
        do {
            arr[i] = arr[i] + 1;
            j = j + 1;
        } while (j < i);
        i = i + 1;
    }
    
    // 2D array with mixed loops
    int matrix[3][4];
    for (i = 0; i < 3; i = i + 1) {
        j = 0;
        while (j < 4) {
            matrix[i][j] = i * 4 + j;
            j = j + 1;
        }
    }
    
    // Read 2D array with do-while
    i = 0;
    do {
        j = 0;
        do {
            sum = sum + matrix[i][j];
            j = j + 1;
        } while (j < 4);
        i = i + 1;
    } while (i < 3);
    
    // Complex condition with all loop types
    for (i = 0; i < 10 && sum < 1000; i = i + 1) {
        sum = sum + i;
    }
    
    i = 0;
    while (i < 10 && sum < 2000) {
        sum = sum + i * 2;
        i = i + 1;
    }
    
    i = 0;
    do {
        sum = sum + i * 3;
        i = i + 1;
    } while (i < 10 && sum < 3000);
    
    // Loop with early exit based on calculation
    for (i = 0; i < 100; i = i + 1) {
        if ((sum + i) > 5000) {
            break;
        }
        sum = sum + i;
    }
    
    // Countdown loops
    for (i = 10; i > 0; i = i - 1) {
        sum = sum + i;
    }
    
    i = 10;
    while (i > 0) {
        sum = sum + i;
        i = i - 1;
    }
    
    i = 10;
    do {
        sum = sum + i;
        i = i - 1;
    } while (i > 0);
    
    // Skip patterns with continue
    for (i = 0; i < 20; i = i + 1) {
        if ((i % 3) == 0) {
            continue;
        }
        sum = sum + i;
    }
    
    i = 0;
    while (i < 20) {
        i = i + 1;
        if ((i % 3) == 0) {
            continue;
        }
        sum = sum + i;
    }
    
    i = 0;
    do {
        i = i + 1;
        if ((i % 3) == 0) {
            continue;
        }
        sum = sum + i;
    } while (i < 20);
    
    // Nested breaks and continues
    for (i = 0; i < 5; i = i + 1) {
        if (i == 2) {
            continue;
        }
        for (j = 0; j < 5; j = j + 1) {
            if (j == 3) {
                break;
            }
            sum = sum + 1;
        }
    }
    
    // Loop modifying multiple variables
    i = 0;
    j = 10;
    while (i < 10 && j > 0) {
        sum = sum + i + j;
        i = i + 1;
        j = j - 1;
    }
    
    // Four levels of nesting
    for (i = 0; i < 2; i = i + 1) {
        j = 0;
        while (j < 2) {
            k = 0;
            do {
                int m = 0;
                while (m < 2) {
                    sum = sum + 1;
                    m = m + 1;
                }
                k = k + 1;
            } while (k < 2);
            j = j + 1;
        }
    }
    
    return sum;
}
