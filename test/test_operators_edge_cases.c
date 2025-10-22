// Edge cases and combined operator scenarios

int main() {
    // Division by powers of 2 (optimization opportunity)
    int a = 100;
    int div2 = a / 2;       // 50
    int div4 = a / 4;       // 25
    int div8 = a / 8;       // 12
    int div16 = a / 16;     // 6
    
    // Modulo edge cases
    int mod1 = 7 % 3;       // 2
    int mod2 = 7 % 7;       // 0 (divisor equals dividend)
    int mod3 = 3 % 7;       // 3 (divisor > dividend)
    int mod4 = 0 % 5;       // 0 (zero dividend)
    int mod5 = -7 % 3;      // -1 (negative dividend)
    int mod6 = 7 % -3;      // 1 (negative divisor)
    int mod7 = -7 % -3;     // -1 (both negative)
    
    // Overflow scenarios (will wrap around in most implementations)
    int large1 = 1000000;
    int large2 = 1000000;
    int overflow = large1 * large2;     // May overflow
    
    // Underflow with division
    int small = 1 / 1000000;            // 0 (integer truncation)
    
    // Chained comparisons with edge values
    int x = 0;
    int cmp1 = (x == 0);                // 1 (true)
    int cmp2 = (x != 0);                // 0 (false)
    int cmp3 = (x > -1);                // 1
    int cmp4 = (x < 1);                 // 1
    
    // Logical short-circuit patterns
    int t1 = 1 && 1 && 1 && 1;          // All true
    int t2 = 1 && 1 && 0 && 1;          // One false makes all false
    int t3 = 0 || 0 || 1 || 0;          // One true makes all true
    int t4 = 0 || 0 || 0 || 0;          // All false
    
    // Negation chains
    int n1 = !0;                        // 1
    int n2 = !!0;                       // 0
    int n3 = !!!0;                      // 1
    int n4 = !!!!0;                     // 0
    int n5 = !5;                        // 0 (any non-zero is true)
    int n6 = !!5;                       // 1
    
    // Bitwise edge cases
    int b1 = 0 & 0xFF;                  // 0
    int b2 = 0xFF & 0xFF;               // 0xFF
    int b3 = 0 | 0;                     // 0
    int b4 = 0xFF | 0;                  // 0xFF
    int b5 = 0 ^ 0;                     // 0
    int b6 = 0xFF ^ 0xFF;               // 0 (XOR with self)
    int b7 = 0xFF ^ 0;                  // 0xFF
    
    // Shift edge cases
    int s1 = 1 << 0;                    // 1 (no shift)
    int s2 = 0 << 5;                    // 0 (shifting zero)
    int s3 = 1 >> 0;                    // 1 (no shift)
    int s4 = 0 >> 5;                    // 0 (shifting zero)
    int s5 = 8 >> 3;                    // 1
    int s6 = 8 >> 4;                    // 0 (shifted all bits out)
    
    // Bitwise NOT with different values
    int bn1 = ~0;                       // -1 (all bits set)
    int bn2 = ~-1;                      // 0 (all bits clear)
    int bn3 = ~1;                       // -2
    int bn4 = ~-2;                      // 1
    
    // Combining unary operators
    int u1 = -~5;                       // -(-6) = 6
    int u2 = ~-5;                       // ~(-5) = 4
    int u3 = !~0;                       // !(-1) = 0
    int u4 = ~!1;                       // ~0 = -1
    
    // Increment/decrement edge cases
    int i = 0;
    int post_inc = i++;                 // post_inc = 0, i = 1
    i = 0;
    int pre_inc = ++i;                  // pre_inc = 1, i = 1
    
    i = 0;
    int post_dec = i--;                 // post_dec = 0, i = -1
    i = 0;
    int pre_dec = --i;                  // pre_dec = -1, i = -1
    
    // Complex precedence with all operator types
    int complex1 = 2 + 3 * 4 << 1 & 0xFF;       // ((2 + (3 * 4)) << 1) & 0xFF
    int complex2 = 1 | 2 & 4 ^ 8 >> 1;          // 1 | ((2 & 4) ^ (8 >> 1))
    int complex3 = !0 && 5 > 3 || 2 < 1;        // ((!0) && (5 > 3)) || (2 < 1)
    
    // Ternary operator edge cases
    int tern1 = 1 ? 10 : 20;            // 10
    int tern2 = 0 ? 10 : 20;            // 20
    int tern3 = 5 ? 100 : 200;          // 100 (non-zero is true)
    int tern4 = -1 ? 30 : 40;           // 30 (negative is true)
    
    // Nested ternary
    int tern5 = 1 ? (2 ? 3 : 4) : 5;    // 3
    int tern6 = 0 ? 1 : (1 ? 2 : 3);    // 2
    
    // Comparison chains
    int chain1 = (5 > 3) > 0;           // 1 > 0 = 1
    int chain2 = (3 > 5) > 0;           // 0 > 0 = 0
    int chain3 = (5 == 5) == 1;         // 1 == 1 = 1
    
    // Mixed signed/unsigned behavior (in C, all treated as int here)
    int m1 = -1 < 1;                    // 1 (true)
    int m2 = -1 > 0;                    // 0 (false)
    int m3 = -5 + 10;                   // 5
    int m4 = -5 * -2;                   // 10
    
    // Expression with all arithmetic operators
    int all_arith = (((10 + 5) * 2 - 8) / 4) % 3;   // ((30 - 8) / 4) % 3 = 5 % 3 = 2
    
    // Expression with all logical operators
    int all_logic = !(0 || 0) && (1 && 1) || (0 && 1);  // 1 && 1 || 0 = 1
    
    // Expression with all bitwise operators
    int all_bits = ((12 & 10) | (5 ^ 3)) << 1 >> 1;     // ((8 | 6) << 1) >> 1 = 14
    
    // Associativity stress test
    int assoc1 = 100 - 50 - 20 - 10;                    // ((100-50)-20)-10 = 20
    int assoc2 = 64 / 8 / 2 / 2;                        // ((64/8)/2)/2 = 2
    int assoc3 = 2 * 3 * 4 * 5;                         // ((2*3)*4)*5 = 120
    
    // Using results in further calculations
    int calc1 = div2 + mod1;            // 50 + 2 = 52
    int calc2 = (cmp1 && cmp3) ? 100 : 200;     // 1 ? 100 : 200 = 100
    int calc3 = tern1 + tern2;          // 10 + 20 = 30
    
    // Boundary value tests
    int boundary1 = 1 - 1;              // 0
    int boundary2 = 0 + 0;              // 0
    int boundary3 = 1 * 1;              // 1
    int boundary4 = 0 * 0;              // 0
    int boundary5 = 1 / 1;              // 1
    
    return 0;
}
