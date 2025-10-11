// Comprehensive do-while loop testing

int main() {
    // Test 1: Simple do-while loop
    int i = 0;
    do {
        i = i + 1;
    } while (i < 5);
    
    // Test 2: do-while with condition false from start
    int j = 10;
    do {
        j = j + 1;
    } while (j < 5);
    
    // Test 3: do-while with decrement
    int k = 5;
    do {
        k = k - 1;
    } while (k > 0);
    
    // Test 4: do-while with multiple statements
    int sum = 0;
    int n = 1;
    do {
        sum = sum + n;
        n = n + 1;
    } while (n <= 5);
    
    // Test 5: Nested do-while loops
    int outer = 0;
    do {
        int inner = 0;
        do {
            inner = inner + 1;
        } while (inner < 3);
        outer = outer + 1;
    } while (outer < 2);
    
    // Test 6: do-while with break
    int m = 0;
    do {
        m = m + 1;
        if (m == 3) {
            break;
        }
    } while (m < 10);
    
    // Test 7: do-while with continue
    int p = 0;
    int count = 0;
    do {
        p = p + 1;
        if (p == 3) {
            continue;
        }
        count = count + 1;
    } while (p < 5);
    
    // Test 8: do-while with complex condition
    int a = 0;
    int b = 10;
    do {
        a = a + 1;
        b = b - 1;
    } while (a < 5 && b > 5);
    
    // Test 9: do-while with logical OR
    int x = 0;
    int y = 0;
    do {
        x = x + 1;
        y = y + 2;
    } while (x < 3 || y < 8);
    
    // Test 10: do-while with array
    int arr[5];
    int idx = 0;
    do {
        arr[idx] = idx * 10;
        idx = idx + 1;
    } while (idx < 5);
    
    // Test 11: do-while reading array
    int total = 0;
    int pos = 0;
    do {
        total = total + arr[pos];
        pos = pos + 1;
    } while (pos < 5);
    
    // Test 12: do-while with all relational operators
    int r1 = 0;
    do {
        r1 = r1 + 1;
    } while (r1 <= 5);
    
    int r2 = 10;
    do {
        r2 = r2 - 1;
    } while (r2 >= 5);
    
    int r3 = 0;
    do {
        r3 = r3 + 1;
    } while (r3 != 5);
    
    // Test 13: Nested do-while with break
    int w = 0;
    do {
        int z = 0;
        do {
            z = z + 1;
            if (z == 2) {
                break;
            }
        } while (z < 5);
        w = w + 1;
    } while (w < 3);
    
    // Test 14: Nested do-while with continue
    int u = 0;
    do {
        int v = 0;
        do {
            v = v + 1;
            if (v == 2) {
                continue;
            }
        } while (v < 3);
        u = u + 1;
    } while (u < 2);
    
    // Test 15: do-while with expression in body
    int base = 2;
    int power = 1;
    int exp = 0;
    do {
        power = power * base;
        exp = exp + 1;
    } while (exp < 4);
    
    // Test 16: do-while with pointer (simple increment)
    int num = 0;
    int *ptr = &num;
    do {
        *ptr = *ptr + 1;
    } while (*ptr < 5);
    
    // Test 17: do-while with character
    char ch = 'A';
    do {
        ch = ch + 1;
    } while (ch < 'E');
    
    // Test 18: do-while with modulo
    int mod = 0;
    do {
        mod = mod + 1;
    } while (mod % 5 != 0);
    
    // Test 19: do-while infinite loop with break
    int inf = 0;
    do {
        inf = inf + 1;
        if (inf > 10) {
            break;
        }
    } while (1);
    
    // Test 20: do-while with multiple conditions
    int c1 = 0;
    int c2 = 0;
    do {
        c1 = c1 + 1;
        c2 = c2 + 2;
    } while (c1 < 5 && c2 < 10 || c1 == 0);
    
    // Test 21: Three level nested do-while
    int l1 = 0;
    do {
        int l2 = 0;
        do {
            int l3 = 0;
            do {
                l3 = l3 + 1;
            } while (l3 < 2);
            l2 = l2 + 1;
        } while (l2 < 2);
        l1 = l1 + 1;
    } while (l1 < 2);
    
    // Test 22: do-while with goto
    int gt = 0;
    do {
        gt = gt + 1;
        if (gt == 5) {
            goto after_loop;
        }
    } while (gt < 10);
    after_loop:
    gt = 100;
    
    // Test 23: do-while with function call (printf)
    int pr = 0;
    do {
        printf("%d\n", pr);
        pr = pr + 1;
    } while (pr < 3);
    
    // Test 24: do-while executing exactly once
    int once = 0;
    do {
        once = once + 1;
    } while (0);
    
    // Test 25: do-while with compound condition and negation
    int neg = 5;
    do {
        neg = neg - 1;
    } while (!(neg == 0));
    
    return 0;
}
