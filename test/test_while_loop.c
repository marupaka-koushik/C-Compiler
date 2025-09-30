// Comprehensive test for while loop statements

int main() {
    int result = 0;
    
    // ========================================
    // TEST 1: Basic while loop
    // ========================================
    int i = 0;
    while (i < 5) {
        result = result + i;
        i++;
    }
    
    // ========================================
    // TEST 2: while loop with decrement
    // ========================================
    int j = 10;
    while (j > 0) {
        result = result + 1;
        j--;
    }
    
    // ========================================
    // TEST 3: while loop with complex condition (AND)
    // ========================================
    int k = 0;
    while ((k < 10) && (k != 5)) {
        result = result + k;
        k++;
    }
    
    // ========================================
    // TEST 4: while loop with logical OR
    // ========================================
    int m = 0;
    while ((m < 3) || (m == 10)) {
        if (m > 5) {
            break;
        }
        result = result + 1;
        m++;
    }
    
    // ========================================
    // TEST 5: Nested while loops
    // ========================================
    int outer = 0;
    while (outer < 3) {
        int inner = 0;
        while (inner < 3) {
            result = result + 1;
            inner++;
        }
        outer++;
    }
    
    // ========================================
    // TEST 6: while loop with break
    // ========================================
    int p = 0;
    while (p < 10) {
        if (p == 5) {
            break;
        }
        result = result + 1;
        p++;
    }
    
    // ========================================
    // TEST 7: while loop with continue
    // ========================================
    int q = 0;
    while (q < 10) {
        q++;
        if (q == 5) {
            continue;
        }
        result = result + 1;
    }
    
    // ========================================
    // TEST 8: while loop with >= condition
    // ========================================
    int r = 10;
    while (r >= 0) {
        result = result + 1;
        r--;
    }
    
    // ========================================
    // TEST 9: while loop with <= condition
    // ========================================
    int s = 0;
    while (s <= 5) {
        result = result + s;
        s++;
    }
    
    // ========================================
    // TEST 10: while loop with != condition
    // ========================================
    int t = 0;
    while (t != 5) {
        result = result + 1;
        t++;
    }
    
    // ========================================
    // TEST 11: while loop with == condition
    // ========================================
    int u = 0;
    while (u == 0) {
        result = result + 1;
        u = 1;
    }
    
    // ========================================
    // TEST 12: while loop with multiple statements
    // ========================================
    int v = 0;
    while (v < 3) {
        int temp1 = v * 2;
        int temp2 = temp1 + 1;
        result = result + temp2;
        v++;
    }
    
    // ========================================
    // TEST 13: while loop with arithmetic in condition
    // ========================================
    int w = 0;
    while ((w + 5) < 10) {
        result = result + w;
        w++;
    }
    
    // ========================================
    // TEST 14: while loop with pre-increment
    // ========================================
    int x = 0;
    while (x < 5) {
        result = result + x;
        ++x;
    }
    
    // ========================================
    // TEST 15: while loop with pre-decrement
    // ========================================
    int y = 10;
    while (y > 5) {
        result = result + 1;
        --y;
    }
    
    // ========================================
    // TEST 16: while loop with compound assignment
    // ========================================
    int z = 0;
    while (z < 10) {
        result = result + z;
        z += 2;
    }
    
    // ========================================
    // TEST 17: while loop with negation
    // ========================================
    int flag = 1;
    while (!flag) {
        result = result + 1;
        flag = 1;
    }
    
    // ========================================
    // TEST 18: while(1) infinite loop with break
    // ========================================
    int counter = 0;
    while (1) {
        counter++;
        if (counter >= 5) {
            break;
        }
        result = result + 1;
    }
    
    // ========================================
    // TEST 19: Nested while with break in inner loop
    // ========================================
    int a = 0;
    while (a < 3) {
        int b = 0;
        while (b < 5) {
            if (b == 3) {
                break;
            }
            result = result + 1;
            b++;
        }
        a++;
    }
    
    // ========================================
    // TEST 20: Nested while with continue in inner loop
    // ========================================
    int c = 0;
    while (c < 3) {
        int d = 0;
        while (d < 5) {
            d++;
            if (d == 3) {
                continue;
            }
            result = result + 1;
        }
        c++;
    }
    
    // ========================================
    // TEST 21: while loop with bitwise condition
    // ========================================
    int e = 12;
    int f = 10;
    while ((e & f) > 0) {
        result = result + 1;
        e--;
    }
    
    // ========================================
    // TEST 22: while loop with complex expression
    // ========================================
    int g = 0;
    int h = 5;
    while ((g * 2) < h) {
        result = result + g;
        g++;
    }
    
    // ========================================
    // TEST 23: Triple nested while loops
    // ========================================
    int l1 = 0;
    while (l1 < 2) {
        int l2 = 0;
        while (l2 < 2) {
            int l3 = 0;
            while (l3 < 2) {
                result = result + 1;
                l3++;
            }
            l2++;
        }
        l1++;
    }
    
    // ========================================
    // TEST 24: while loop with multiple conditions
    // ========================================
    int n1 = 0;
    int n2 = 10;
    while ((n1 < 5) && (n2 > 5)) {
        result = result + 1;
        n1++;
        n2--;
    }
    
    // ========================================
    // TEST 25: while loop with if-else inside
    // ========================================
    int o = 0;
    while (o < 10) {
        if (o > 5) {
            result = result + o;
        } else {
            result = result + 1;
        }
        o++;
    }
    
    return result;
}
