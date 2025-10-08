// Advanced if-else edge cases and complex scenarios

int main() {
    // Dangling else problem (else associates with nearest if)
    int a = 10;
    int b = 20;
    int c = 5;
    int result = 0;
    
    // Classic dangling else
    if (a > 0)
        if (b > 0)
            result = 1;
        else
            result = 2;  // This else belongs to inner if
    
    // Dangling else with explicit braces
    if (a > 0) {
        if (b > 0)
            result = 3;
    } else {
        result = 4;  // This else belongs to outer if
    }
    
    // Complex nesting patterns
    if (a > 0) {
        if (b > 15) {
            if (c < 10) {
                result = 5;
            } else {
                result = 6;
            }
        } else {
            if (c > 0) {
                result = 7;
            }
        }
    } else {
        if (b > 0) {
            result = 8;
        } else {
            if (c != 0) {
                result = 9;
            } else {
                result = 10;
            }
        }
    }
    
    // All possible paths in nested if-else
    if (a > 0) {
        if (b > 0) {
            result = 11;  // Path 1: a>0, b>0
        } else {
            result = 12;  // Path 2: a>0, b<=0
        }
    } else {
        if (b > 0) {
            result = 13;  // Path 3: a<=0, b>0
        } else {
            result = 14;  // Path 4: a<=0, b<=0
        }
    }
    
    // Testing with negative numbers
    int neg = -10;
    if (neg < 0) {
        if (neg < -5) {
            if (neg < -15) {
                result = 15;
            } else {
                result = 16;
            }
        } else {
            result = 17;
        }
    } else {
        result = 18;
    }
    
    // Testing with zero
    int zero = 0;
    if (zero) {
        result = 19;  // Should not execute
    } else {
        if (!zero) {
            result = 20;  // Should execute
        }
    }
    
    // Multiple variables in condition
    if (a > 0 && b > 0 && c > 0) {
        result = 21;
    } else if (a < 0 && b < 0 && c < 0) {
        result = 22;
    } else if (a == 0 || b == 0 || c == 0) {
        result = 23;
    } else {
        result = 24;
    }
    
    // Symmetric conditions
    if (a < b) {
        result = 25;
    } else if (a > b) {
        result = 26;
    } else {  // a == b
        result = 27;
    }
    
    // Three-way comparison
    if (a < b && b < c) {
        result = 28;
    } else if (a < c && c < b) {
        result = 29;
    } else if (b < a && a < c) {
        result = 30;
    } else if (b < c && c < a) {
        result = 31;
    } else if (c < a && a < b) {
        result = 32;
    } else if (c < b && b < a) {
        result = 33;
    } else {
        result = 34;  // Some are equal
    }
    
    // Bitwise operations in nested conditions
    int flags = 7;  // 0111 in binary
    if (flags & 1) {  // Bit 0 set
        if (flags & 2) {  // Bit 1 set
            if (flags & 4) {  // Bit 2 set
                result = 35;
            } else {
                result = 36;
            }
        } else {
            result = 37;
        }
    } else {
        result = 38;
    }
    
    // XOR in condition
    if ((a > 0) ^ (b > 0)) {  // Exactly one is positive
        result = 39;
    } else {
        result = 40;
    }
    
    // Shift operations in condition
    if ((a << 1) > b) {
        result = 41;
    }
    
    if ((b >> 1) < a) {
        result = 42;
    }
    
    // Modulo patterns
    if (a % 2 == 0) {  // Even
        if (b % 2 == 0) {  // Both even
            result = 43;
        } else {  // a even, b odd
            result = 44;
        }
    } else {  // a odd
        if (b % 2 == 0) {  // a odd, b even
            result = 45;
        } else {  // Both odd
            result = 46;
        }
    }
    
    // Division in conditions
    if (a / 2 > b / 2) {
        result = 47;
    }
    
    // Complex arithmetic expressions
    if ((a * 2 + b * 3 - c * 4) > 0) {
        result = 48;
    }
    
    if (((a + b) * (a - b)) > 0) {
        result = 49;
    }
    
    // Parentheses in conditions
    if ((a > 0) && ((b > 10) || (c < 5))) {
        result = 50;
    }
    
    if (((a > 0) && (b > 10)) || (c < 5)) {
        result = 51;
    }
    
    // NOT with complex expressions
    if (!(a > 0 && b > 0)) {
        result = 52;
    }
    
    if (!((a > 0) || (b > 0))) {
        result = 53;
    }
    
    // Double negation
    if (!!(a > 0)) {
        result = 54;
    }
    
    // Multiple assignments in if branches
    int x = 0;
    int y = 0;
    if (a > b) {
        x = a;
        y = b;
        result = x + y;
    } else {
        x = b;
        y = a;
        result = x - y;
    }
    
    // Cascading assignments
    if (a > 0) {
        result = x = y = 100;
    } else {
        result = x = y = 200;
    }
    
    // Using increment/decrement in branches
    int counter = 10;
    if (counter > 5) {
        counter = counter + 1;
        result = counter;
    } else {
        counter = counter - 1;
        result = counter;
    }
    
    // Post increment in condition evaluation
    int p = 0;
    if (p++ < 1) {
        result = 60;  // p was 0, now 1
    }
    
    // Pre increment in condition
    int q = 0;
    if (++q > 0) {
        result = 61;  // q is now 1
    }
    
    // Combining pre and post increment
    int m = 0;
    int n = 0;
    if (++m > n++) {
        result = 62;
    }
    
    // Ternary operator in if condition
    if ((a > b ? a : b) > 15) {
        result = 63;
    }
    
    // Ternary in if body
    if (a > 0) {
        result = (b > 0 ? 64 : 65);
    } else {
        result = (b > 0 ? 66 : 67);
    }
    
    // Nested ternary in condition
    if ((a > 0 ? (b > 0 ? 1 : 0) : 0)) {
        result = 68;
    }
    
    // Comparison results used in further conditions
    int cmp1 = (a > b);
    int cmp2 = (b > c);
    if (cmp1 && cmp2) {
        result = 69;
    }
    
    // All relational operators
    if (a < b && b <= 20 && a > 0 && a >= 5 && a != 15 && b == 20) {
        result = 70;
    }
    
    // Short-circuit evaluation edge cases
    if (0 && (result = 100)) {  // result should NOT be assigned
        result = 101;
    }
    
    if (1 || (result = 102)) {  // result should NOT be assigned
        result = 103;
    }
    
    // Comma operator in condition (if supported)
    // if ((x = 5, y = 10, x < y)) {
    //     result = 104;
    // }
    
    // Very deep nesting (10 levels)
    if (a > 0) {
        if (b > 0) {
            if (c > 0) {
                if (a < 20) {
                    if (b < 30) {
                        if (c < 10) {
                            if (a + b > 20) {
                                if (b - c > 10) {
                                    if (a * 2 > 15) {
                                        if (b / 2 < 15) {
                                            result = 200;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    return result;
}
