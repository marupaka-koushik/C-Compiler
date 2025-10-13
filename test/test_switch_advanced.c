// Advanced switch-case edge cases and fall-through patterns

int main() {
    int result = 0;
    int x = 3;
    
    // Complete fall-through (no breaks at all)
    switch (x) {
        case 1:
            result = result + 1;
        case 2:
            result = result + 2;
        case 3:
            result = result + 3;
        case 4:
            result = result + 4;
        case 5:
            result = result + 5;
        default:
            result = result + 100;
    }
    // For x=3: adds 3+4+5+100=112
    
    // Partial fall-through
    switch (x) {
        case 1:
            result = 10;
        case 2:
            result = result + 20;
            break;
        case 3:
            result = 30;
        case 4:
            result = result + 40;
        case 5:
            result = result + 50;
            break;
        default:
            result = 0;
            break;
    }
    
    // Fall-through with default at end
    switch (x) {
        case 1:
        case 2:
        case 3:
        default:
            result = 100;
            break;
        case 4:
            result = 200;
            break;
    }
    
    // Fall-through with operations
    int sum = 0;
    switch (x) {
        case 5:
            sum = sum + 5;
        case 4:
            sum = sum + 4;
        case 3:
            sum = sum + 3;
        case 2:
            sum = sum + 2;
        case 1:
            sum = sum + 1;
            break;
        case 0:
            sum = 0;
            break;
    }
    
    // Switch with array index
    int arr[10];
    arr[0] = 100;
    arr[1] = 200;
    arr[2] = 300;
    
    int idx = 1;
    switch (idx) {
        case 0:
            result = arr[0];
            break;
        case 1:
            result = arr[1];
            break;
        case 2:
            result = arr[2];
            break;
        default:
            result = -1;
            break;
    }
    
    // Nested switch with fall-through
    int a = 1;
    int b = 2;
    switch (a) {
        case 1:
            switch (b) {
                case 1:
                    result = 11;
                case 2:
                    result = result + 12;
                    break;
                case 3:
                    result = 13;
                    break;
            }
        case 2:
            result = result + 20;
            break;
        default:
            result = 0;
            break;
    }
    
    // Switch in nested loops
    int i, j;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            switch (i + j) {
                case 0:
                    result = result + 1;
                    break;
                case 1:
                    result = result + 2;
                    break;
                case 2:
                    result = result + 3;
                    break;
                default:
                    result = result + 10;
                    break;
            }
        }
    }
    
    // Switch with break to outer loop
    for (i = 0; i < 10; i = i + 1) {
        switch (i) {
            case 0:
                result = result + 1;
                break;
            case 5:
                result = result + 5;
                break;  // This breaks switch, not loop
            case 9:
                result = result + 9;
                break;
        }
        if (i == 7) {
            break;  // This breaks the loop
        }
    }
    
    // Switch with continue in loop
    for (i = 0; i < 5; i = i + 1) {
        switch (i) {
            case 2:
                continue;  // Skip rest of loop iteration
            case 3:
                result = result + 10;
                break;
            default:
                result = result + 1;
                break;
        }
        result = result + 100;  // Skipped when i=2
    }
    
    // Switch with return
    int test = 5;
    switch (test) {
        case 5:
            result = 500;
            // No break here, but we can return
            // return result;
            break;
        default:
            result = 0;
            break;
    }
    
    // Switch with all same result (fall-through)
    switch (x) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            result = 999;
            break;
    }
    
    // Switch with calculations in cases
    int base = 100;
    switch (x) {
        case 1:
            result = base + x;
            break;
        case 2:
            result = base + x * 2;
            break;
        case 3:
            result = base + x * 3;
            break;
        default:
            result = base;
            break;
    }
    
    // Switch with array operations
    int matrix[3][3];
    switch (x) {
        case 1:
            matrix[0][0] = 1;
            matrix[0][1] = 2;
            break;
        case 2:
            matrix[1][0] = 3;
            matrix[1][1] = 4;
            break;
        case 3:
            matrix[2][0] = 5;
            matrix[2][1] = 6;
            break;
    }
    
    // Switch with bitwise operations
    int flags = 7;  // 0111
    switch (flags & 3) {  // Get lower 2 bits
        case 0:  // 00
            result = 0;
            break;
        case 1:  // 01
            result = 1;
            break;
        case 2:  // 10
            result = 2;
            break;
        case 3:  // 11
            result = 3;
            break;
    }
    
    // Switch with shift operation
    switch (x >> 1) {
        case 0:
            result = 0;
            break;
        case 1:
            result = 10;
            break;
        case 2:
            result = 20;
            break;
        default:
            result = -1;
            break;
    }
    
    // Switch with modulo pattern (odd/even)
    switch (x % 2) {
        case 0:  // Even
            result = 100;
            break;
        case 1:  // Odd
            result = 200;
            break;
    }
    
    // Switch with range-like behavior using fall-through
    int score = 85;
    int grade = 0;
    
    // Map score to grade category
    if (score >= 90) grade = 9;
    else if (score >= 80) grade = 8;
    else if (score >= 70) grade = 7;
    else grade = 0;
    
    switch (grade) {
        case 9:
        case 10:
            result = 90;  // A grade
            break;
        case 8:
            result = 80;  // B grade
            break;
        case 7:
            result = 70;  // C grade
            break;
        default:
            result = 0;   // F grade
            break;
    }
    
    // Switch with ternary in expression
    int val = 5;
    switch (val > 3 ? 1 : 0) {
        case 0:
            result = 0;
            break;
        case 1:
            result = 100;
            break;
    }
    
    // Deep nesting: switch in switch in switch
    switch (x) {
        case 1:
            switch (x + 1) {
                case 2:
                    switch (x + 2) {
                        case 3:
                            result = 123;
                            break;
                        default:
                            result = 100;
                            break;
                    }
                    break;
                default:
                    result = 10;
                    break;
            }
            break;
        default:
            result = 1;
            break;
    }
    
    // Switch with mixed break positions
    switch (x) {
        case 1:
            result = 10;
            if (result > 5) {
                break;  // Early break
            }
            result = 20;  // Unreachable when result > 5
            break;
        case 2:
            result = 30;
            break;
        default:
            result = 0;
            break;
    }
    
    return result;
}
