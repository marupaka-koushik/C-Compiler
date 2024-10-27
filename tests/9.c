// Test Suite for Jump Statements (converted to while loops)
// Features: goto, break, continue  
// Returns: 0 on Success, Line Number on Failure

int main() {
    int i, j, sum, check;

    // Test 1.1: Break in WHILE loop (was FOR loop)
    // Loop 0 to 10, but break at 5.
    // Sum: 0 + 1 + 2 + 3 + 4 = 10
    sum = 0;
    i = 0;
    while (i < 10) {
        if (i == 5) {
            break;
        }
        sum = sum + i;
        i = i + 1;
    }
    if (sum != 10) {
        return 24;
    }
    if (i != 5) {
        return 27;
    }

    // Test 1.2: Simple Break in WHILE loop
    i = 0;
    while (1) {
        i = i + 1;
        if (i == 10) {
            break;
        }
    }
    if (i != 10) {
        return 39;
    }

    // Test 1.3: Nested Break
    sum = 0;
    i = 0;
    while (i < 3) {
        j = 0;
        while (j < 5) {
            if (j == 2) {
                break;
            }
            sum = sum + 1;
            j = j + 1;
        }
        i = i + 1;
    }
    if (sum != 6) {
        return 59;
    }

    // Test 2.1: Continue in WHILE loop (was FOR loop)
    sum = 0;
    i = 0;
    while (i < 5) {
        if (i == 2) {
            i = i + 1;
            continue;
        }
        sum = sum + i;
        i = i + 1;
    }
    if (sum != 8) {
        return 74;
    }

    // Test 2.2: Continue in WHILE loop
    i = 0;
    sum = 0;
    while (i < 5) {
        i = i + 1;
        if (i == 3) {
            continue;
        }
        sum = sum + i;
    }
    if (sum != 12) {
        return 90;
    }

    // Test 2.3: Nested Continue
    sum = 0;
    i = 0;
    while (i < 3) {
        j = 0;
        while (j < 3) {
            if (j == 1) {
                j = j + 1;
                continue;
            }
            sum = sum + 1;
            j = j + 1;
        }
        i = i + 1;
    }
    if (sum != 6) {
        return 107;
    }

    // Test 3.1: Forward Jump (Skipping Code)
    sum = 0;
    goto SKIP_MATH;
    sum = 100;
    
SKIP_MATH:
    if (sum != 0) {
        return 120;
    }

    // Test 3.2: Backward Jump (Manual Loop)
    i = 0;
    sum = 0;
LABEL_LOOP:
    sum = sum + i;
    i = i + 1;
    if (i < 4) {
        goto LABEL_LOOP;
    }
    if (sum != 6) {
        return 133;
    }

    // Test 3.3: Escaping Nested Loops
    sum = 0;
    i = 0;
    while (i < 10) {
        j = 0;
        while (j < 10) {
            sum = sum + 1;
            if (sum == 5) {
                goto ESCAPE_ALL;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return 147;

ESCAPE_ALL:
    if (sum != 5) {
        return 151;
    }

    // Test 4.1: Label right before return
    goto END_CHECK;
    return 160;

END_CHECK:
    check = 1;
    if (check != 1) {
        return 164;
    }

    // Test 4.2: Break inside nested 'if' inside 'loop'
    sum = 0;
    while (1) {
        if (1) {
            if (1) {
                break;
            }
        }
        return 174;
    }
    
    return 0;
}
