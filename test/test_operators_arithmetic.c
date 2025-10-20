// Comprehensive test for arithmetic operators and edge cases

int main() {
    // Basic arithmetic operators
    int a = 10;
    int b = 20;
    int c;
    
    // Addition
    c = a + b;              // 30
    c = a + 5;              // 15
    c = 100 + 200;          // 300
    
    // Subtraction
    c = b - a;              // 10
    c = a - 15;             // -5 (negative result)
    c = 5 - 10;             // -5 (negative result)
    c = 0 - a;              // -10 (unary minus equivalent)
    
    // Multiplication
    c = a * b;              // 200
    c = a * 0;              // 0 (multiply by zero)
    c = a * 1;              // 10 (identity)
    c = a * -1;             // -10 (negative)
    c = 3 * 4 * 5;          // 60 (chained)
    
    // Division
    c = b / a;              // 2 (integer division)
    c = 20 / 3;             // 6 (truncation)
    c = 100 / 10;           // 10
    c = 1 / 2;              // 0 (truncates to zero)
    c = -20 / 3;            // -6 (negative division)
    c = 20 / -3;            // -6
    c = -20 / -3;           // 6
    
    // Modulus
    c = b % a;              // 0
    c = 20 % 3;             // 2
    c = 17 % 5;             // 2
    c = 5 % 10;             // 5 (when divisor > dividend)
    c = -20 % 3;            // -2 (negative modulus)
    c = 20 % -3;            // 2
    
    // Unary plus and minus
    c = +a;                 // 10
    c = -a;                 // -10
    c = -(-a);              // 10 (double negative)
    c = -(a + b);           // -30
    
    // Increment/Decrement operators
    int x = 5;
    int y;
    
    // Prefix increment
    y = ++x;                // x=6, y=6
    
    // Postfix increment
    y = x++;                // y=6, x=7
    
    // Prefix decrement
    y = --x;                // x=6, y=6
    
    // Postfix decrement
    y = x--;                // y=6, x=5
    
    // Complex expressions with increment/decrement
    int i = 0;
    int result;
    result = ++i + ++i;     // i=2, result depends on evaluation order
    
    i = 5;
    result = i++ * 2;       // result=10, i=6
    
    i = 5;
    result = ++i * 2;       // result=12, i=6
    
    // Operator precedence tests
    c = 2 + 3 * 4;          // 14 (multiplication first)
    c = 2 * 3 + 4;          // 10
    c = 10 - 3 - 2;         // 5 (left-to-right)
    c = 20 / 2 / 2;         // 5 (left-to-right)
    c = 2 + 3 * 4 - 5;      // 9
    
    // Parentheses to override precedence
    c = (2 + 3) * 4;        // 20
    c = 2 * (3 + 4);        // 14
    c = (10 - 3) - 2;       // 5
    c = 10 - (3 - 2);       // 9
    
    // Mixed operations
    c = a + b * 2;          // 50
    c = (a + b) * 2;        // 60
    c = a * b / 2;          // 100
    c = a / 2 * b;          // 100 (left-to-right)
    c = 100 / 10 * 2;       // 20
    c = 100 / (10 * 2);     // 5
    
    // Chained assignments
    int p, q, r;
    p = q = r = 5;          // All get 5
    
    // Assignment with arithmetic
    p = 10;
    p = p + 5;              // 15
    p = p * 2;              // 30
    p = p / 3;              // 10
    p = p - 1;              // 9
    p = p % 4;              // 1
    
    // Compound assignment operators
    int n = 10;
    n = n + 5;              // 15 (equivalent to n += 5)
    n = n - 3;              // 12
    n = n * 2;              // 24
    n = n / 4;              // 6
    n = n % 4;              // 2
    
    // Edge case: multiple operations in one expression
    int expr1 = 2 + 3 * 4 - 6 / 2;           // 11
    int expr2 = (2 + 3) * (4 - 1);           // 15
    int expr3 = 10 + 20 / 5 - 2 * 3;         // 8
    
    // Negative numbers in expressions
    int neg1 = -5 + 10;     // 5
    int neg2 = -5 - 10;     // -15
    int neg3 = -5 * -3;     // 15
    int neg4 = -20 / -4;    // 5
    
    // Zero in operations
    int z = 0;
    int z1 = z + 10;        // 10
    int z2 = z - 10;        // -10
    int z3 = z * 100;       // 0
    int z4 = 0 / 10;        // 0
    
    // Operations with 1
    int one = 1;
    int o1 = 50 * 1;        // 50
    int o2 = 50 / 1;        // 50
    
    return 0;
}
