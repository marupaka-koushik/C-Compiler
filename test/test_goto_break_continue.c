// Comprehensive goto, break, and continue testing

int main() {
    // ========== BREAK TESTS ==========
    
    // Test 1: break in for loop
    int i;
    for (i = 0; i < 10; i = i + 1) {
        if (i == 5) {
            break;
        }
        int val = i;
    }
    
    // Test 2: break in while loop
    int count = 0;
    while (count < 10) {
        if (count == 3) {
            break;
        }
        count = count + 1;
    }
    
    // Test 3: break in nested for loop (breaks inner)
    int j;
    int k;
    for (j = 0; j < 3; j = j + 1) {
        for (k = 0; k < 3; k = k + 1) {
            if (k == 1) {
                break;
            }
            int temp = k;
        }
    }
    
    // Test 4: break in nested while
    int outer = 0;
    while (outer < 3) {
        int inner = 0;
        while (inner < 3) {
            if (inner == 1) {
                break;
            }
            inner = inner + 1;
        }
        outer = outer + 1;
    }
    
    // Test 5: Multiple breaks in same loop
    int m;
    for (m = 0; m < 20; m = m + 1) {
        if (m == 5) {
            break;
        }
        if (m == 10) {
            break;
        }
        int x = m;
    }
    
    // ========== CONTINUE TESTS ==========
    
    // Test 6: continue in for loop
    int n;
    for (n = 0; n < 5; n = n + 1) {
        if (n == 2) {
            continue;
        }
        int y = n;
    }
    
    // Test 7: continue in while loop
    int w = 0;
    while (w < 5) {
        w = w + 1;
        if (w == 3) {
            continue;
        }
        int z = w;
    }
    
    // Test 8: continue in nested for loop
    int p;
    int q;
    for (p = 0; p < 3; p = p + 1) {
        for (q = 0; q < 3; q = q + 1) {
            if (q == 1) {
                continue;
            }
            int val2 = q;
        }
    }
    
    // Test 9: Multiple continues in same loop
    int r;
    for (r = 0; r < 10; r = r + 1) {
        if (r < 3) {
            continue;
        }
        if (r > 7) {
            continue;
        }
        int val3 = r;
    }
    
    // Test 10: Mix of break and continue
    int s;
    for (s = 0; s < 20; s = s + 1) {
        if (s < 5) {
            continue;
        }
        if (s > 15) {
            break;
        }
        int val4 = s;
    }
    
    // ========== GOTO TESTS ==========
    
    // Test 11: Simple goto forward
    int a = 0;
    goto label1;
    a = 10;
    label1:
    a = 20;
    
    // Test 12: goto backward (simple loop)
    int b = 0;
    label2:
    b = b + 1;
    if (b < 5) {
        goto label2;
    }
    
    // Test 13: goto skip code
    int c = 0;
    goto skip1;
    c = 100;
    c = 200;
    skip1:
    c = 300;
    
    // Test 14: Multiple labels
    int d = 0;
    goto point1;
    d = 1;
    point1:
    d = 2;
    goto point2;
    d = 3;
    point2:
    d = 4;
    
    // Test 15: goto in if statement
    int e = 5;
    if (e > 0) {
        goto positive;
    }
    e = -1;
    positive:
    e = 1;
    
    // Test 16: goto out of loop
    int f;
    for (f = 0; f < 10; f = f + 1) {
        if (f == 3) {
            goto exit_loop;
        }
    }
    exit_loop:
    f = 100;
    
    // Test 17: break with condition
    int g;
    for (g = 0; g < 10; g = g + 1) {
        if (g % 2 == 0) {
            break;
        }
    }
    
    // Test 18: continue with condition
    int h;
    for (h = 0; h < 10; h = h + 1) {
        if (h % 2 == 0) {
            continue;
        }
        int odd = h;
    }
    
    // Test 19: break in do-while (if supported)
    int cnt = 0;
    while (cnt < 10) {
        cnt = cnt + 1;
        if (cnt == 5) {
            break;
        }
    }
    
    // Test 20: continue in do-while
    int cnt2 = 0;
    while (cnt2 < 10) {
        cnt2 = cnt2 + 1;
        if (cnt2 == 5) {
            continue;
        }
        int v = cnt2;
    }
    
    // Test 21: Nested goto and loops
    int level = 0;
    for (level = 0; level < 5; level = level + 1) {
        if (level == 3) {
            goto finish;
        }
    }
    finish:
    level = 99;
    
    // Test 22: goto to skip multiple statements
    int skip = 0;
    goto end_section;
    skip = 1;
    skip = 2;
    skip = 3;
    skip = 4;
    end_section:
    skip = 5;
    
    // Test 23: break with complex condition
    int arr[5];
    int idx;
    for (idx = 0; idx < 5; idx = idx + 1) {
        arr[idx] = idx * 10;
        if (arr[idx] > 25) {
            break;
        }
    }
    
    // Test 24: continue with complex condition
    int sum = 0;
    int num;
    for (num = 0; num < 10; num = num + 1) {
        if (num % 3 == 0) {
            continue;
        }
        sum = sum + num;
    }
    
    // Test 25: goto across different scopes
    {
        int local = 10;
        goto outer_label;
    }
    outer_label:
    int after = 20;
    
    return 0;
}
