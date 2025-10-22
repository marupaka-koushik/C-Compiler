// Comprehensive test for operator precedence and associativity

int main() {
    int result;
    int a = 5;
    int b = 10;
    int c = 15;
    
    // Precedence level tests
    
    // Multiplicative (*, /, %) before Additive (+, -)
    result = 2 + 3 * 4;             // 14 not 20
    result = 20 - 10 / 2;           // 15 not 5
    result = 3 * 4 + 5;             // 17
    result = 15 / 3 - 2;            // 3
    result = 10 % 3 + 5;            // 6 (1 + 5)
    result = 2 + 15 % 4;            // 5 (2 + 3)
    
    // Shift operators after Additive
    result = 1 << 2 + 1;            // 1 << 3 = 8
    result = 16 >> 1 + 1;           // 16 >> 2 = 4
    result = 2 + 3 << 1;            // 5 << 1 = 10
    
    // Relational (<, <=, >, >=) after Shift
    result = 1 << 2 < 8;            // 4 < 8 = 1
    result = 3 + 2 > 4;             // 5 > 4 = 1
    result = 10 - 5 >= 5;           // 5 >= 5 = 1
    
    // Equality (==, !=) after Relational
    result = 3 < 5 == 1;            // 1 == 1 = 1
    result = 2 + 2 == 4;            // 4 == 4 = 1
    result = 5 > 3 != 0;            // 1 != 0 = 1
    
    // Bitwise AND after Equality
    result = 1 == 1 & 1;            // 1 == (1 & 1) = 1
    result = 12 & 10 == 8;          // (12 & 10) == 8 = 1
    
    // Bitwise XOR after Bitwise AND
    result = 12 & 10 ^ 4;           // 8 ^ 4 = 12
    result = 15 ^ 7 & 3;            // 15 ^ (7 & 3) = 12
    
    // Bitwise OR after Bitwise XOR
    result = 8 ^ 4 | 2;             // 12 | 2 = 14
    result = 1 | 2 ^ 3;             // 1 | (2 ^ 3) = 1
    
    // Logical AND (&&) after Bitwise OR
    result = 1 | 0 && 1;            // (1 | 0) && 1 = 1
    result = 1 && 2 | 4;            // 1 && (2 | 4) = 1
    
    // Logical OR (||) after Logical AND
    result = 0 && 1 || 1;           // 0 || 1 = 1
    result = 1 || 0 && 0;           // 1 || 0 = 1
    
    // Complex precedence tests
    result = 2 + 3 * 4 - 5 / 2;             // 2 + 12 - 2 = 12
    result = 10 % 3 + 5 * 2 - 8 / 4;        // 1 + 10 - 2 = 9
    result = 1 << 2 + 1 >> 1;               // (1 << 3) >> 1 = 4
    result = 3 + 2 < 10 && 5 > 2;           // (5 < 10) && (5 > 2) = 1
    result = 2 * 3 > 5 || 4 < 2;            // (6 > 5) || (4 < 2) = 1
    
    // Parentheses override precedence
    result = (2 + 3) * 4;           // 20 not 14
    result = 20 - (10 / 2);         // 15
    result = (3 * 4) + 5;           // 17 (same as without)
    result = 3 * (4 + 5);           // 27 not 17
    result = (1 << 2) + 1;          // 5
    result = 1 << (2 + 1);          // 8
    
    // Associativity tests
    
    // Left-to-right associativity
    result = 20 - 10 - 5;           // (20 - 10) - 5 = 5, not 20 - (10 - 5) = 15
    result = 20 / 10 / 2;           // (20 / 10) / 2 = 1, not 20 / (10 / 2) = 4
    result = 16 % 5 % 2;            // (16 % 5) % 2 = 1
    result = 100 - 50 + 20;         // (100 - 50) + 20 = 70
    result = 100 / 10 * 5;          // (100 / 10) * 5 = 50
    
    // Right-to-left associativity (assignment)
    int x, y, z;
    x = y = z = 10;                 // z = 10, y = 10, x = 10
    
    // Unary operators (right-to-left)
    result = -~5;                   // -(~5) = -(-6) = 6
    result = !-5;                   // !(-5) = 0
    result = ~!0;                   // ~(!0) = ~1 = -2
    
    // Mixed associativity
    int p = 5;
    int q = 10;
    result = p + q - 3;             // (5 + 10) - 3 = 12 (left-to-right)
    
    // Ternary operator (right-to-left associativity)
    result = 1 ? 2 : 3 ? 4 : 5;     // 1 ? 2 : (3 ? 4 : 5) = 2
    result = 0 ? 1 : 1 ? 2 : 3;     // 0 ? 1 : (1 ? 2 : 3) = 2
    
    // Complex expressions with multiple precedence levels
    result = 5 + 3 * 2 - 8 / 4 % 3;         // 5 + 6 - 2 = 9
    result = (5 + 3) * 2 - 8 / (4 % 3);     // 16 - 8 = 8
    result = 1 << 2 + 1 & 7;                // (1 << 3) & 7 = 0
    result = (1 << 2) + (1 & 7);            // 4 + 1 = 5
    
    // Logical operators with other operators
    result = 5 > 3 && 10 < 20;              // 1 && 1 = 1
    result = 2 + 3 == 5 || 4 < 2;           // 1 || 0 = 1
    result = !(5 > 10) && (3 < 8);          // 1 && 1 = 1
    result = 5 + 2 > 6 && 3 * 2 < 10;       // 1 && 1 = 1
    
    // Bitwise and logical together
    result = 5 & 3 && 10 | 2;               // (5 & 3) && (10 | 2) = 1
    result = 1 | 2 && 3 ^ 1;                // (1 | 2) && (3 ^ 1) = 1
    result = (5 & 3) == 1 || (10 | 2) == 10;    // 1 || 1 = 1
    
    // Increment/decrement in expressions
    int i = 5;
    result = ++i * 2;               // 6 * 2 = 12, i = 6
    i = 5;
    result = i++ * 2;               // 5 * 2 = 10, i = 6
    i = 5;
    result = 2 * ++i;               // 2 * 6 = 12, i = 6
    i = 5;
    result = 2 * i++;               // 2 * 5 = 10, i = 6
    
    // Nested parentheses
    result = ((2 + 3) * (4 + 5)) / (3 * 3);     // (5 * 9) / 9 = 5
    result = (((1 + 2) * 3) + 4) * 5;           // ((3 * 3) + 4) * 5 = 65
    
    // Mixing all operator types
    result = 2 + 3 * 4 > 10 && 15 / 3 == 5;     // (2 + 12 > 10) && (5 == 5) = 1
    result = 1 << 2 | 3 & 2 ^ 1;                // 4 | (3 & 2) ^ 1 = 4 | 2 ^ 1 = 7
    result = !0 && 5 > 3 || 2 + 2 == 5;         // (1 && 1) || 0 = 1
    
    return 0;
}
