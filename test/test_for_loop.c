// Comprehensive test for for-loop statements

int main() {
    int result = 0;
    
    // ========================================
    // TEST 1: Basic for loop with increment
    // ========================================
    for (int i = 0; i < 5; i++) {
        result = result + i;
    }
    
    // ========================================
    // TEST 2: for loop with decrement
    // ========================================
    for (int j = 10; j > 0; j--) {
        result = result + 1;
    }
    
    // ========================================
    // TEST 3: for loop with step size
    // ========================================
    for (int k = 0; k < 10; k = k + 2) {
        result = result + k;
    }
    
    // ========================================
    // TEST 4: for loop with complex condition
    // ========================================
    for (int m = 0; (m < 10) && (m != 5); m++) {
        result = result + m;
    }
    
    // ========================================
    // TEST 5: for loop with compound assignment
    // ========================================
    for (int n = 1; n < 100; n *= 2) {
        result = result + n;
    }
    
    // ========================================
    // TEST 6: Nested for loops
    // ========================================
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result = result + 1;
        }
    }
    
    // ========================================
    // TEST 7: for loop with multiple init variables
    // ========================================
    int a = 0;
    int b = 0;
    for (a = 0; a < 5; a++) {
        b = b + a;
    }
    result = result + b;
    
    // ========================================
    // TEST 8: for loop with empty body
    // ========================================
    for (int p = 0; p < 5; p++) {
        // Empty body
    }
    
    // ========================================
    // TEST 9: for loop with break
    // ========================================
    for (int q = 0; q < 10; q++) {
        if (q == 5) {
            break;
        }
        result = result + 1;
    }
    
    // ========================================
    // TEST 10: for loop with continue
    // ========================================
    for (int r = 0; r < 10; r++) {
        if (r == 5) {
            continue;
        }
        result = result + 1;
    }
    
    // ========================================
    // TEST 11: for loop with >= condition
    // ========================================
    for (int s = 10; s >= 0; s--) {
        result = result + 1;
    }
    
    // ========================================
    // TEST 12: for loop with <= condition
    // ========================================
    for (int t = 0; t <= 5; t++) {
        result = result + t;
    }
    
    // ========================================
    // TEST 13: for loop with != condition
    // ========================================
    for (int u = 0; u != 5; u++) {
        result = result + 1;
    }
    
    // ========================================
    // TEST 14: for loop with arithmetic in increment
    // ========================================
    for (int v = 0; v < 20; v = v + 3) {
        result = result + v;
    }
    
    // ========================================
    // TEST 15: Nested for loops with break
    // ========================================
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (j == 2) {
                break;
            }
            result = result + 1;
        }
    }
    
    // ========================================
    // TEST 16: for loop with complex increment
    // ========================================
    for (int w = 0; w < 10; w = w * 2 + 1) {
        result = result + 1;
    }
    
    // ========================================
    // TEST 17: for loop with pre-increment
    // ========================================
    for (int x = 0; x < 5; ++x) {
        result = result + x;
    }
    
    // ========================================
    // TEST 18: for loop with pre-decrement
    // ========================================
    for (int y = 10; y > 5; --y) {
        result = result + 1;
    }
    
    // ========================================
    // TEST 19: for loop with OR condition
    // ========================================
    for (int z = 0; (z < 5) || (z == 10); z++) {
        if (z > 5) {
            break;
        }
        result = result + 1;
    }
    
    // ========================================
    // TEST 20: for loop with multiple statements
    // ========================================
    for (int aa = 0; aa < 3; aa++) {
        int temp1 = aa * 2;
        int temp2 = temp1 + 1;
        result = result + temp2;
    }
    
    // ========================================
    // TEST 21: Triple nested for loops
    // ========================================
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                result = result + 1;
            }
        }
    }
    
    // ========================================
    // TEST 22: for loop with existing variable
    // ========================================
    int existingVar = 0;
    for (existingVar = 0; existingVar < 5; existingVar++) {
        result = result + existingVar;
    }
    
    // ========================================
    // TEST 23: for loop with complex body
    // ========================================
    for (int bb = 0; bb < 5; bb++) {
        if (bb > 2) {
            result = result + bb;
        } else {
            result = result + 1;
        }
    }
    
    // ========================================
    // TEST 24: for loop counting down
    // ========================================
    for (int cc = 10; cc > 0; cc = cc - 2) {
        result = result + cc;
    }
    
    // ========================================
    // TEST 25: for loop with compound operations
    // ========================================
    for (int dd = 1; dd < 10; dd += 2) {
        result = result + dd;
    }
    
    return result;
}
