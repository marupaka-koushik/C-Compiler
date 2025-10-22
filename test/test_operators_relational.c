// Comprehensive test for relational and logical operators

int main() {
    int a = 10;
    int b = 20;
    int c = 10;
    int result;
    
    // Relational operators: == (equal to)
    result = (a == b);      // 0 (false)
    result = (a == c);      // 1 (true)
    result = (10 == 10);    // 1
    result = (5 == 6);      // 0
    
    // Relational operators: != (not equal to)
    result = (a != b);      // 1 (true)
    result = (a != c);      // 0 (false)
    result = (10 != 10);    // 0
    result = (5 != 6);      // 1
    
    // Relational operators: < (less than)
    result = (a < b);       // 1 (true)
    result = (b < a);       // 0 (false)
    result = (a < c);       // 0 (equal, not less)
    result = (5 < 10);      // 1
    result = (10 < 5);      // 0
    result = (10 < 10);     // 0
    
    // Relational operators: > (greater than)
    result = (b > a);       // 1 (true)
    result = (a > b);       // 0 (false)
    result = (a > c);       // 0 (equal, not greater)
    result = (10 > 5);      // 1
    result = (5 > 10);      // 0
    result = (10 > 10);     // 0
    
    // Relational operators: <= (less than or equal)
    result = (a <= b);      // 1 (true)
    result = (b <= a);      // 0 (false)
    result = (a <= c);      // 1 (equal counts as true)
    result = (5 <= 10);     // 1
    result = (10 <= 10);    // 1
    result = (15 <= 10);    // 0
    
    // Relational operators: >= (greater than or equal)
    result = (b >= a);      // 1 (true)
    result = (a >= b);      // 0 (false)
    result = (a >= c);      // 1 (equal counts as true)
    result = (10 >= 5);     // 1
    result = (10 >= 10);    // 1
    result = (5 >= 10);     // 0
    
    // Logical AND (&&)
    result = (1 && 1);      // 1 (both true)
    result = (1 && 0);      // 0 (one false)
    result = (0 && 1);      // 0
    result = (0 && 0);      // 0 (both false)
    result = (5 && 10);     // 1 (non-zero = true)
    result = (0 && 100);    // 0
    
    // Logical OR (||)
    result = (1 || 1);      // 1
    result = (1 || 0);      // 1 (at least one true)
    result = (0 || 1);      // 1
    result = (0 || 0);      // 0 (both false)
    result = (5 || 0);      // 1
    result = (0 || 0);      // 0
    
    // Logical NOT (!)
    result = !0;            // 1 (NOT false)
    result = !1;            // 0 (NOT true)
    result = !5;            // 0 (any non-zero is true)
    result = !-5;           // 0
    result = !!1;           // 1 (double negation)
    result = !!0;           // 0
    
    // Combining relational and logical operators
    result = (a < b) && (b > c);        // 1 && 1 = 1
    result = (a == c) && (b == 20);     // 1 && 1 = 1
    result = (a > b) && (c < b);        // 0 && 1 = 0
    result = (a < b) || (a > b);        // 1 || 0 = 1
    result = (a == b) || (c == 10);     // 0 || 1 = 1
    result = (a > b) || (b < c);        // 0 || 0 = 0
    
    // NOT with relational operators
    result = !(a < b);      // 0 (NOT true)
    result = !(a > b);      // 1 (NOT false)
    result = !(a == c);     // 0
    result = !(a != c);     // 1
    
    // Complex logical expressions
    result = (a < b) && (b < 30) && (c == 10);      // 1 && 1 && 1 = 1
    result = (a > 5) && (b > 15) && (c > 15);       // 1 && 1 && 0 = 0
    result = (a < 5) || (b > 15) || (c == 10);      // 0 || 1 || 1 = 1
    result = (a < 5) || (b < 15) || (c > 15);       // 0 || 0 || 0 = 0
    
    // Mixed AND and OR (precedence: AND before OR)
    result = 0 || 1 && 1;   // 0 || (1 && 1) = 1
    result = 1 && 0 || 1;   // (1 && 0) || 1 = 1
    result = 0 && 0 || 1;   // (0 && 0) || 1 = 1
    result = 1 || 1 && 0;   // 1 || (1 && 0) = 1
    
    // Parentheses to override precedence
    result = (0 || 1) && 1; // 1 && 1 = 1
    result = 1 && (0 || 1); // 1 && 1 = 1
    result = (1 && 0) || 1; // 0 || 1 = 1
    
    // Chained comparisons (evaluated left-to-right)
    result = (a < b) == (c < b);        // 1 == 1 = 1
    result = (a == c) != (a == b);      // 1 != 0 = 1
    
    // Negation of complex expressions
    result = !((a < b) && (c == 10));   // !(1 && 1) = 0
    result = !((a > b) || (c > 10));    // !(0 || 0) = 1
    
    // Using results in further operations
    int x = (a < b);        // x = 1
    int y = (a > b);        // y = 0
    result = x && y;        // 0
    result = x || y;        // 1
    result = !x;            // 0
    
    // Relational operators with negative numbers
    int neg1 = -5;
    int neg2 = -10;
    result = (neg1 > neg2);     // 1 (-5 > -10)
    result = (neg1 < neg2);     // 0
    result = (neg1 < 0);        // 1
    result = (neg1 > 0);        // 0
    result = (neg1 == -5);      // 1
    
    // Zero comparisons
    result = (0 == 0);      // 1
    result = (0 != 0);      // 0
    result = (0 < 1);       // 1
    result = (0 > 1);       // 0
    result = (0 <= 0);      // 1
    result = (0 >= 0);      // 1
    
    // Expressions as operands
    result = ((a + 5) == (c + 5));          // 1
    result = ((a * 2) > (b - 5));           // 1 (20 > 15)
    result = ((b / 2) == a);                // 1 (10 == 10)
    result = ((a + b) >= (c * 3));          // 1 (30 >= 30)
    
    // Short-circuit evaluation (not directly testable, but important)
    // In (0 && anything), 'anything' should not be evaluated
    // In (1 || anything), 'anything' should not be evaluated
    
    return 0;
}
