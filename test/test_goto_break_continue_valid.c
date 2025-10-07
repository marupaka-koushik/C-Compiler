// Comprehensive test for goto, break, continue - VALID cases

int main() {
    // ========== GOTO TESTS ==========
    
    // 1. Simple goto forward
    int x = 0;
    goto label1;
    x = 100;  // This should be skipped
label1:
    x = 10;
    
    // 2. Goto backward
    int count = 0;
loop_start:
    count = count + 1;
    if (count < 3) {
        goto loop_start;
    }
    
    // 3. Goto to skip code
    int y = 5;
    if (y > 0) {
        goto skip_negative;
    }
    y = -1;  // This won't execute
skip_negative:
    y = y + 1;
    
    // 4. Goto in nested blocks
    int a = 0;
    if (a == 0) {
        goto inner_label;
        a = 99;  // Skipped
inner_label:
        a = 1;
    }
    
    // 5. Multiple goto labels
    int choice = 2;
    if (choice == 1) {
        goto option1;
    } else if (choice == 2) {
        goto option2;
    } else {
        goto option3;
    }
    
option1:
    x = 1;
    goto end_options;
    
option2:
    x = 2;
    goto end_options;
    
option3:
    x = 3;
    
end_options:
    x = x + 0;
    
    // ========== BREAK TESTS ==========
    
    // 6. Break in for loop
    int i;
    for (i = 0; i < 10; i = i + 1) {
        if (i == 5) {
            break;
        }
    }
    
    // 7. Break in while loop
    int j = 0;
    while (j < 10) {
        if (j == 3) {
            break;
        }
        j = j + 1;
    }
    
    // 8. Break in nested loops (breaks inner loop)
    int m, n;
    for (m = 0; m < 5; m = m + 1) {
        for (n = 0; n < 5; n = n + 1) {
            if (n == 2) {
                break;  // Breaks inner loop only
            }
        }
    }
    
    // 9. Break in switch
    int val = 2;
    switch (val) {
        case 1:
            x = 1;
            break;
        case 2:
            x = 2;
            break;
        case 3:
            x = 3;
            break;
        default:
            x = 0;
            break;
    }
    
    // 10. Multiple breaks in loop
    int k = 0;
    while (k < 100) {
        if (k == 10) {
            break;
        }
        if (k == 20) {
            break;
        }
        k = k + 1;
    }
    
    // 11. Break after goto
    for (i = 0; i < 10; i = i + 1) {
        if (i == 3) {
            goto break_label;
        }
break_label:
        if (i >= 3) {
            break;
        }
    }
    
    // ========== CONTINUE TESTS ==========
    
    // 12. Continue in for loop
    int sum = 0;
    for (i = 0; i < 10; i = i + 1) {
        if (i == 5) {
            continue;  // Skip when i == 5
        }
        sum = sum + i;
    }
    
    // 13. Continue in while loop
    int p = 0;
    int even_sum = 0;
    while (p < 10) {
        p = p + 1;
        if (p % 2 == 1) {
            continue;  // Skip odd numbers
        }
        even_sum = even_sum + p;
    }
    
    // 14. Continue in nested loops
    int total = 0;
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            if (j == i) {
                continue;  // Skip when j == i
            }
            total = total + 1;
        }
    }
    
    // 15. Multiple continues in loop
    int filtered = 0;
    for (i = 0; i < 20; i = i + 1) {
        if (i < 5) {
            continue;  // Skip first 5
        }
        if (i > 15) {
            continue;  // Skip after 15
        }
        filtered = filtered + 1;
    }
    
    // 16. Continue with goto
    for (i = 0; i < 10; i = i + 1) {
        if (i == 3) {
            goto cont_label;
        }
cont_label:
        if (i == 7) {
            continue;
        }
        sum = sum + 1;
    }
    
    // ========== COMBINED TESTS ==========
    
    // 17. Break and continue in same loop
    int result = 0;
    for (i = 0; i < 100; i = i + 1) {
        if (i < 10) {
            continue;  // Skip first 10
        }
        if (i > 50) {
            break;  // Stop at 50
        }
        result = result + i;
    }
    
    // 18. Goto, break, continue together
    int complex = 0;
    for (i = 0; i < 20; i = i + 1) {
        if (i == 5) {
            goto special;
        }
        if (i % 2 == 0) {
            continue;
        }
        complex = complex + i;
special:
        if (i > 15) {
            break;
        }
    }
    
    // 19. Break in switch inside loop
    for (i = 0; i < 5; i = i + 1) {
        switch (i) {
            case 0:
                x = 0;
                break;  // Breaks switch, not loop
            case 1:
                x = 1;
                break;
            default:
                x = -1;
                break;
        }
    }
    
    // 20. Nested loops with break and continue
    int outer_sum = 0;
    for (i = 0; i < 10; i = i + 1) {
        if (i % 2 == 0) {
            continue;  // Skip even in outer loop
        }
        for (j = 0; j < 10; j = j + 1) {
            if (j % 2 == 0) {
                continue;  // Skip even in inner loop
            }
            if (j > 5) {
                break;  // Break inner loop when j > 5
            }
            outer_sum = outer_sum + 1;
        }
    }
    
    // 21. Goto across control structures
    int state = 0;
    if (state == 0) {
        goto state1;
    }
    
state1:
    for (i = 0; i < 5; i = i + 1) {
        if (i == 3) {
            goto state2;
        }
    }
    
state2:
    state = 1;
    
    // 22. Break in do-while loop
    int dw = 0;
    do {
        if (dw == 5) {
            break;
        }
        dw = dw + 1;
    } while (dw < 10);
    
    // 23. Continue in do-while loop
    int dw2 = 0;
    do {
        dw2 = dw2 + 1;
        if (dw2 % 3 == 0) {
            continue;
        }
        sum = sum + dw2;
    } while (dw2 < 15);
    
    // 24. Complex control flow
    for (i = 0; i < 100; i = i + 1) {
        if (i < 10) {
            continue;
        }
        
        if (i == 25) {
            goto midpoint;
        }
        
        if (i == 50) {
            goto endpoint;
        }
        
        result = result + 1;
        continue;  // Explicit continue
        
midpoint:
        result = result + 10;
    }
    
endpoint:
    result = result + 100;
    
    // 25. All three in one complex function
    int final_result = 0;
    for (i = 0; i < 30; i = i + 1) {
        if (i == 0) {
            goto init;
        }
        
        if (i % 5 == 0) {
            continue;
        }
        
        if (i > 25) {
            break;
        }
        
        final_result = final_result + i;
        
init:
        if (i == 0) {
            final_result = 0;
        }
    }
    
    return 0;
}
