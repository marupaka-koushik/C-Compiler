// Comprehensive test for if-else statements and edge cases

int main() {
    int x = 10;
    int y = 20;
    int result = 0;
    
    // Basic if statement
    if (x < y) {
        result = 1;
    }
    
    // Basic if-else
    if (x > y) {
        result = 2;
    } else {
        result = 3;
    }
    
    // if-else with equality
    if (x == y) {
        result = 4;
    } else {
        result = 5;
    }
    
    // if-else with inequality
    if (x != y) {
        result = 6;
    } else {
        result = 7;
    }
    
    // if with zero condition (false)
    if (0) {
        result = 8;
    }
    
    // if with non-zero condition (true)
    if (1) {
        result = 9;
    }
    
    // if with variable as condition
    if (x) {
        result = 10;
    }
    
    // if with negative number (true)
    if (-5) {
        result = 11;
    }
    
    // if with zero variable (false)
    int zero = 0;
    if (zero) {
        result = 12;
    } else {
        result = 13;
    }
    
    // if-else if chain
    if (x < 5) {
        result = 14;
    } else if (x < 10) {
        result = 15;
    } else if (x < 15) {
        result = 16;
    } else {
        result = 17;
    }
    
    // if-else if-else if chain (multiple conditions)
    if (x == 0) {
        result = 18;
    } else if (x == 5) {
        result = 19;
    } else if (x == 10) {
        result = 20;
    } else if (x == 15) {
        result = 21;
    } else {
        result = 22;
    }
    
    // Nested if statements
    if (x > 0) {
        if (y > 0) {
            result = 23;
        }
    }
    
    // Nested if-else
    if (x > 0) {
        if (y > 10) {
            result = 24;
        } else {
            result = 25;
        }
    } else {
        result = 26;
    }
    
    // Three levels of nesting
    if (x > 0) {
        if (y > 0) {
            if (x < y) {
                result = 27;
            }
        }
    }
    
    // Complex nested if-else
    if (x > 0) {
        if (y > 15) {
            result = 28;
        } else {
            if (y > 10) {
                result = 29;
            } else {
                result = 30;
            }
        }
    } else {
        if (x < -5) {
            result = 31;
        } else {
            result = 32;
        }
    }
    
    // Logical AND in condition
    if (x > 0 && y > 0) {
        result = 33;
    }
    
    // Logical OR in condition
    if (x < 0 || y < 0) {
        result = 34;
    } else {
        result = 35;
    }
    
    // Complex logical expression
    if ((x > 0 && y > 0) || (x < 0 && y < 0)) {
        result = 36;
    }
    
    // Logical NOT in condition
    if (!(x > y)) {
        result = 37;
    }
    
    // Multiple conditions with AND
    if (x > 5 && x < 15 && y > 10) {
        result = 38;
    }
    
    // Multiple conditions with OR
    if (x == 0 || x == 10 || x == 20) {
        result = 39;
    }
    
    // Mixed AND and OR
    if ((x > 0 && y > 0) || (x == 0)) {
        result = 40;
    }
    
    // Arithmetic in condition
    if (x + y > 25) {
        result = 41;
    }
    
    // Complex arithmetic in condition
    if ((x * 2) + (y / 2) > 20) {
        result = 42;
    }
    
    // Modulo in condition
    if (x % 2 == 0) {
        result = 43;
    } else {
        result = 44;
    }
    
    // Bitwise operations in condition
    if (x & 1) {
        result = 45;
    } else {
        result = 46;
    }
    
    // Comparison chain
    if (x < y && y < 30) {
        result = 47;
    }
    
    // Negation of complex condition
    if (!(x > 5 && y < 15)) {
        result = 48;
    }
    
    // Assignment in if body
    if (x > 0) {
        result = x + 10;
    }
    
    // Multiple statements in if
    if (x < y) {
        result = x;
        result = result + 1;
        result = result * 2;
    }
    
    // Multiple statements in else
    if (x > y) {
        result = 100;
    } else {
        result = y;
        result = result - 1;
        result = result / 2;
    }
    
    // Empty if body (edge case)
    if (x > 100) {
        // Nothing
    }
    
    // if without braces (single statement)
    if (x > 0)
        result = 50;
    
    // if-else without braces
    if (x > y)
        result = 51;
    else
        result = 52;
    
    // Nested if without braces
    if (x > 0)
        if (y > 0)
            result = 53;
    
    // if with function call result (simulated with expression)
    if ((x + y) > 0) {
        result = 54;
    }
    
    // Chained comparisons in condition
    if ((x > 5) == (y > 15)) {
        result = 55;
    }
    
    // Using previous result in condition
    if (result > 50) {
        result = result - 50;
    }
    
    // if-else modifying same variable
    if (x == 10) {
        x = x + 1;
    } else {
        x = x - 1;
    }
    
    // Conditional increment
    if (y > 15) {
        y = y + 1;
    }
    
    // Post-increment in condition
    int i = 0;
    if (i++) {
        result = 60;
    } else {
        result = 61;
    }
    
    // Pre-increment in condition
    int j = 0;
    if (++j) {
        result = 62;
    } else {
        result = 63;
    }
    
    // Ternary in condition (already an expression)
    if ((x > 0 ? 1 : 0)) {
        result = 64;
    }
    
    // Compound condition with all operators
    if ((x > 0) && ((y % 2) == 0) || (x == 10)) {
        result = 65;
    }
    
    // Deeply nested if-else (5 levels)
    if (x > 0) {
        if (y > 0) {
            if (x < y) {
                if (y < 30) {
                    if (x > 5) {
                        result = 70;
                    }
                }
            }
        }
    }
    
    // Ladder of else-if (many conditions)
    if (x == 1) {
        result = 71;
    } else if (x == 2) {
        result = 72;
    } else if (x == 3) {
        result = 73;
    } else if (x == 4) {
        result = 74;
    } else if (x == 5) {
        result = 75;
    } else if (x == 10) {
        result = 76;
    } else {
        result = 77;
    }
    
    // if with goto (if supported)
    // Skipping as goto may not be implemented
    
    // Using variables declared in if
    if (x > 0) {
        int temp = x * 2;
        result = temp;
    }
    
    // Side effects in condition
    int counter = 0;
    if ((counter = counter + 1) > 0) {
        result = 80;
    }
    
    // Multiple conditions testing same variable
    if (x > 0 && x < 20 && x != 15) {
        result = 81;
    }
    
    // Range check
    if (x >= 10 && x <= 20) {
        result = 82;
    }
    
    // Boundary conditions
    if (x == 0) {
        result = 83;
    } else if (x == 1) {
        result = 84;
    } else if (x == -1) {
        result = 85;
    }
    
    return 0;
}
