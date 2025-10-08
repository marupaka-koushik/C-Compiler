// Comprehensive test for if-else statements

int main() {
    int x = 10;
    int y = 20;
    int result = 0;
    
    // ========================================
    // TEST 1: Simple if statement (no else)
    // ========================================
    if (x < y) {
        result = 1;
    }
    
    // ========================================
    // TEST 2: if-else statement
    // ========================================
    if (x > y) {
        result = 2;
    } else {
        result = 3;
    }
    
    // ========================================
    // TEST 3: if-else with complex condition
    // ========================================
    if ((x < y) && (x > 5)) {
        result = 4;
    } else {
        result = 5;
    }
    
    // ========================================
    // TEST 4: if-else with logical OR
    // ========================================
    if ((x == 10) || (y == 30)) {
        result = 6;
    } else {
        result = 7;
    }
    
    // ========================================
    // TEST 5: Nested if-else
    // ========================================
    if (x < y) {
        if (x > 5) {
            result = 8;
        } else {
            result = 9;
        }
    } else {
        result = 10;
    }
    
    // ========================================
    // TEST 6: if-else-if ladder
    // ========================================
    if (x > 20) {
        result = 11;
    } else if (x > 10) {
        result = 12;
    } else if (x > 5) {
        result = 13;
    } else {
        result = 14;
    }
    
    // ========================================
    // TEST 7: if with multiple statements
    // ========================================
    if (x != y) {
        int a = 5;
        int b = 10;
        result = a + b;
    }
    
    // ========================================
    // TEST 8: if-else with expressions in condition
    // ========================================
    if ((x + 5) > (y - 5)) {
        result = 15;
    } else {
        result = 16;
    }
    
    // ========================================
    // TEST 9: if with negation
    // ========================================
    if (!(x > y)) {
        result = 17;
    }
    
    // ========================================
    // TEST 10: if with relational operators
    // ========================================
    if (x <= 10) {
        result = 18;
    }
    
    if (x >= 10) {
        result = 19;
    }
    
    if (x == 10) {
        result = 20;
    }
    
    if (x != 20) {
        result = 21;
    }
    
    // ========================================
    // TEST 11: Empty if/else blocks
    // ========================================
    if (x > 100) {
        // Empty if block
    } else {
        result = 22;
    }
    
    // ========================================
    // TEST 12: Complex nested conditions
    // ========================================
    if ((x > 5) && (y < 30)) {
        if ((x == 10) || (y == 20)) {
            result = 23;
        }
    } else {
        if (x < 0) {
            result = 24;
        } else {
            result = 25;
        }
    }
    
    // ========================================
    // TEST 13: if-else with arithmetic in blocks
    // ========================================
    if (x < y) {
        result = x + y;
    } else {
        result = x - y;
    }
    
    // ========================================
    // TEST 14: if with bitwise conditions
    // ========================================
    if ((x & y) > 0) {
        result = 26;
    }
    
    // ========================================
    // TEST 15: Chained if statements
    // ========================================
    if (x > 0) {
        result = 27;
    }
    if (y > 0) {
        result = 28;
    }
    
    return result;
}
