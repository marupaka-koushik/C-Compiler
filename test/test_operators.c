// Comprehensive test for all untested operators

int main() {
    // ========================================
    // BITWISE OPERATORS
    // ========================================
    int a = 12;     // 1100 in binary
    int b = 10;     // 1010 in binary
    
    // Bitwise AND (&)
    int bitwiseAnd = a & b;      // Should be 8 (1000)
    
    // Bitwise OR (|)
    int bitwiseOr = a | b;       // Should be 14 (1110)
    
    // Bitwise XOR (^)
    int bitwiseXor = a ^ b;      // Should be 6 (0110)
    
    // Left Shift (<<)
    int leftShift = a << 2;      // Should be 48 (110000)
    
    // Right Shift (>>)
    int rightShift = a >> 2;     // Should be 3 (0011)
    
    // Bitwise NOT (~)
    int bitwiseNot = ~a;         // Should be -13
    
    // ========================================
    // COMPOUND ASSIGNMENT OPERATORS
    // ========================================
    int x = 20;
    int y = 5;
    
    // Addition assignment (+=)
    x += y;                      // x = 20 + 5 = 25
    
    // Subtraction assignment (-=)
    x -= 3;                      // x = 25 - 3 = 22
    
    // Multiplication assignment (*=)
    y *= 4;                      // y = 5 * 4 = 20
    
    // Division assignment (/=)
    y /= 2;                      // y = 20 / 2 = 10
    
    // Modulo assignment (%=)
    int mod = 17;
    mod %= 5;                    // mod = 17 % 5 = 2
    
    // Bitwise AND assignment (&=)
    int p = 15;
    p &= 7;                      // p = 15 & 7 = 7
    
    // Bitwise OR assignment (|=)
    int q = 8;
    q |= 4;                      // q = 8 | 4 = 12
    
    // Bitwise XOR assignment (^=)
    int r = 10;
    r ^= 6;                      // r = 10 ^ 6 = 12
    
    // Left shift assignment (<<=)
    int s = 3;
    s <<= 2;                     // s = 3 << 2 = 12
    
    // Right shift assignment (>>=)
    int t = 16;
    t >>= 2;                     // t = 16 >> 2 = 4
    
    // ========================================
    // LOGICAL OPERATORS
    // ========================================
    int flag1 = 1;
    int flag2 = 0;
    
    // Logical OR (||)
    if (flag1 || flag2) {
        x = 100;
    }
    
    // Logical NOT (!)
    if (!flag2) {
        y = 200;
    }
    
    // Combined logical operators
    if ((a > 5) || (b < 20)) {
        int combined = 1;
    }
    
    // ========================================
    // RELATIONAL OPERATORS (untested ones)
    // ========================================
    
    // Greater than or equal (>=)
    if (x >= 25) {
        int ge_test = 1;
    }
    
    // Equal to (==)
    if (y == 200) {
        int eq_test = 1;
    }
    
    // ========================================
    // DECREMENT OPERATOR
    // ========================================
    int counter = 10;
    
    // Post-decrement
    int post_dec = counter--;    // post_dec = 10, counter = 9
    
    // Pre-decrement
    int pre_dec = --counter;     // pre_dec = 8, counter = 8
    
    // ========================================
    // UNARY OPERATORS
    // ========================================
    int num = -15;
    
    // Unary plus
    int pos = +num;              // pos = -15
    
    // Unary minus (negation)
    int neg = -num;              // neg = 15
    
    // ========================================
    // COMPLEX EXPRESSIONS
    // ========================================
    
    // Mix of bitwise and arithmetic
    int complex1 = (a & b) + (a | b);
    
    // Mix of logical and relational
    int complex2 = (x > y) && (p != q);
    
    // Compound assignment with bitwise
    int val = 20;
    val &= 15;
    val |= 8;
    
    return 0;
}
