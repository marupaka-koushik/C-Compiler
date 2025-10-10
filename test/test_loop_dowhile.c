// Comprehensive test for do-while loops with edge cases

int main() {
    int i = 0;
    int sum = 0;
    int result = 0;
    
    // Basic do-while loop
    i = 0;
    do {
        sum = sum + i;
        i = i + 1;
    } while (i < 10);
    
    // Do-while counting down
    i = 10;
    do {
        sum = sum + i;
        i = i - 1;
    } while (i > 0);
    
    // Do-while with different step
    i = 0;
    do {
        sum = sum + i;
        i = i + 2;
    } while (i < 20);
    
    // Do-while - exactly one iteration (condition false)
    i = 10;
    do {
        sum = sum + 100;
        i = i + 1;
    } while (i < 5);  // Executes once even though condition is false
    
    // Do-while with complex condition
    i = 0;
    do {
        sum = sum + i;
        i = i + 1;
    } while (i < 10 && i * i < 50);
    
    // Do-while with OR condition
    i = 0;
    do {
        sum = sum + 1;
        i = i + 1;
        if (i > 20) {
            break;
        }
    } while (i < 5 || sum < 100);
    
    // Do-while with break
    i = 0;
    do {
        if (i == 10) {
            break;
        }
        sum = sum + 1;
        i = i + 1;
    } while (i < 100);
    
    // Do-while with continue
    i = 0;
    do {
        i = i + 1;
        if (i == 5) {
            continue;
        }
        sum = sum + 1;
    } while (i < 10);
    
    // Do-while with multiple breaks
    i = 0;
    do {
        if (i == 5) {
            break;
        }
        if (i == 10) {
            break;
        }
        sum = sum + 1;
        i = i + 1;
    } while (i < 20);
    
    // Do-while with multiple continues
    i = 0;
    do {
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
    } while (i < 10);
    
    // Nested do-while loops
    i = 0;
    do {
        int j = 0;
        do {
            sum = sum + 1;
            j = j + 1;
        } while (j < 5);
        i = i + 1;
    } while (i < 5);
    
    // Nested do-while with different conditions
    i = 0;
    do {
        int j = 0;
        do {
            sum = sum + i + j;
            j = j + 1;
        } while (j < 4);
        i = i + 1;
    } while (i < 3);
    
    // Triple nested do-while loops
    i = 0;
    do {
        int j = 0;
        do {
            int k = 0;
            do {
                sum = sum + 1;
                k = k + 1;
            } while (k < 2);
            j = j + 1;
        } while (j < 2);
        i = i + 1;
    } while (i < 2);
    
    // Do-while with array
    int arr[10];
    i = 0;
    do {
        arr[i] = i * 2;
        i = i + 1;
    } while (i < 10);
    
    // Do-while reading array
    i = 0;
    do {
        sum = sum + arr[i];
        i = i + 1;
    } while (i < 10);
    
    // Do-while with break in nested loop
    i = 0;
    do {
        int j = 0;
        do {
            if (j == 3) {
                break;
            }
            sum = sum + 1;
            j = j + 1;
        } while (j < 5);
        i = i + 1;
    } while (i < 5);
    
    // Do-while with continue in nested loop
    i = 0;
    do {
        int j = 0;
        do {
            j = j + 1;
            if (j == 2) {
                continue;
            }
            sum = sum + 1;
        } while (j < 5);
        i = i + 1;
    } while (i < 3);
    
    // Do-while with false condition (always runs once)
    i = 100;
    do {
        sum = sum + 50;
        i = i + 1;
    } while (0);  // Runs exactly once
    
    // Do-while with true condition (requires break)
    i = 0;
    do {
        sum = sum + 1;
        i = i + 1;
        if (i >= 10) {
            break;
        }
    } while (1);
    
    // Do-while with variable as condition
    i = 10;
    do {
        sum = sum + 1;
        i = i - 1;
    } while (i);
    
    // Do-while with negative numbers
    i = -10;
    do {
        sum = sum + 1;
        i = i + 1;
    } while (i < 0);
    
    // Do-while with <= condition
    i = 0;
    do {
        sum = sum + 1;
        i = i + 1;
    } while (i <= 10);
    
    // Do-while with >= condition
    i = 10;
    do {
        sum = sum + 1;
        i = i - 1;
    } while (i >= 0);
    
    // Do-while with != condition
    i = 0;
    do {
        sum = sum + 1;
        i = i + 1;
    } while (i != 10);
    
    // Do-while with == condition
    i = 0;
    do {
        sum = sum + 100;
        i = 1;
    } while (i == 0);  // Runs once
    
    // Do-while without braces (single statement)
    i = 0;
    do
        i = i + 1;
    while (i < 5);
    
    // Do-while searching for value
    i = 0;
    int found = 0;
    do {
        if (arr[i] == 10) {
            found = 1;
            break;
        }
        i = i + 1;
    } while (i < 10);
    
    // Do-while computing factorial
    int n = 5;
    int factorial = 1;
    i = 1;
    do {
        factorial = factorial * i;
        i = i + 1;
    } while (i <= n);
    
    // Do-while finding maximum
    i = 0;
    int max_val = arr[0];
    do {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
        i = i + 1;
    } while (i < 10);
    
    // Do-while finding minimum
    i = 0;
    int min_val = arr[0];
    do {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
        i = i + 1;
    } while (i < 10);
    
    // Do-while with accumulation
    i = 0;
    int total = 0;
    do {
        total = total + arr[i];
        i = i + 1;
    } while (i < 10);
    
    // Do-while with modulo
    i = 0;
    do {
        if ((i % 2) == 0) {
            sum = sum + i;
        }
        i = i + 1;
    } while (i < 20);
    
    // Do-while with bitwise operations
    i = 1;
    do {
        sum = sum + 1;
        i = i << 1;
    } while (i < 64);
    
    // Do-while modifying loop variable in multiple places
    i = 0;
    do {
        if (i == 10) {
            i = i + 5;  // Skip ahead
        }
        sum = sum + 1;
        i = i + 1;
    } while (i < 20);
    
    // Do-while with 2D array
    int matrix[3][3];
    i = 0;
    do {
        int j = 0;
        do {
            matrix[i][j] = i * 3 + j;
            j = j + 1;
        } while (j < 3);
        i = i + 1;
    } while (i < 3);
    
    // Do-while reading 2D array
    i = 0;
    do {
        int j = 0;
        do {
            sum = sum + matrix[i][j];
            j = j + 1;
        } while (j < 3);
        i = i + 1;
    } while (i < 3);
    
    // Do-while with logical NOT
    i = 0;
    do {
        sum = sum + 1;
        i = i + 1;
    } while (!(i >= 10));
    
    // Do-while with complex arithmetic condition
    i = 0;
    do {
        sum = sum + i;
        i = i + 1;
    } while ((i * 2 + 5) < 25);
    
    // Do-while with bitwise AND in condition
    i = 15;
    do {
        sum = sum + i;
        i = i - 2;
    } while (i & 1);
    
    // Do-while guaranteed to run (key difference from while)
    i = 1000;
    do {
        sum = sum + 999;  // Runs once even though i already >= 100
    } while (i < 100);
    
    result = sum;
    return result;
}
