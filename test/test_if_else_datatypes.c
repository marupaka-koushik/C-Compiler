// if-else with various data types and complex conditions

int main() {
    // Testing with different integer values
    int zero = 0;
    int one = 1;
    int negOne = -1;
    int large = 1000000;
    int result = 0;
    
    // Zero tests
    if (zero) {
        result = 1;  // Should not execute
    } else {
        result = 2;  // Should execute
    }
    
    if (!zero) {
        result = 3;  // Should execute
    }
    
    // One tests
    if (one) {
        result = 4;  // Should execute
    }
    
    if (one == 1) {
        result = 5;
    }
    
    // Negative number tests
    if (negOne) {
        result = 6;  // Should execute (non-zero)
    }
    
    if (negOne < 0) {
        result = 7;
    }
    
    // Large number tests
    if (large) {
        result = 8;
    }
    
    if (large > 999999) {
        result = 9;
    }
    
    // Pointer-like tests (using integers as addresses)
    int addr = 0;
    if (addr) {
        result = 10;  // NULL-like
    } else {
        result = 11;
    }
    
    addr = 1234;
    if (addr) {
        result = 12;  // Non-NULL
    }
    
    // Array index conditions
    int arr[10];
    int idx = 0;
    if (idx >= 0 && idx < 10) {
        arr[idx] = 100;
        result = 13;
    }
    
    idx = 5;
    if (idx > 0 && idx < 10) {
        arr[idx] = 200;
        result = 14;
    }
    
    idx = 15;
    if (idx >= 10) {
        result = 15;  // Out of bounds
    } else {
        arr[idx] = 300;
        result = 16;
    }
    
    // Multi-dimensional array conditions
    int matrix[3][3];
    int row = 1;
    int col = 2;
    if (row < 3 && col < 3) {
        matrix[row][col] = 999;
        result = 17;
    }
    
    // Struct-related conditions (using simple fields)
    int structField1 = 10;
    int structField2 = 20;
    if (structField1 < structField2) {
        result = 18;
    }
    
    // Enum-like constants
    int RED = 0;
    int GREEN = 1;
    int BLUE = 2;
    int color = 1;
    
    if (color == RED) {
        result = 19;
    } else if (color == GREEN) {
        result = 20;
    } else if (color == BLUE) {
        result = 21;
    } else {
        result = 22;
    }
    
    // Multiple enum checks
    if (color >= RED && color <= BLUE) {
        result = 23;
    }
    
    // Typedef-like usage (just int)
    int Integer = 100;
    if (Integer > 50) {
        result = 24;
    }
    
    // Testing with calculations
    int a = 10;
    int b = 20;
    
    // Sum condition
    if ((a + b) == 30) {
        result = 25;
    }
    
    // Difference condition
    if ((b - a) == 10) {
        result = 26;
    }
    
    // Product condition
    if ((a * b) == 200) {
        result = 27;
    }
    
    // Division condition
    if ((b / a) == 2) {
        result = 28;
    }
    
    // Modulo condition
    if ((b % a) == 0) {
        result = 29;
    }
    
    // Complex calculation
    if (((a + b) * 2 - a) == 50) {
        result = 30;
    }
    
    // Testing ranges
    int val = 15;
    
    // Lower bound
    if (val >= 10) {
        if (val <= 20) {
            result = 31;  // In range [10, 20]
        }
    }
    
    // Upper bound
    if (val > 20) {
        result = 32;
    } else if (val < 10) {
        result = 33;
    } else {
        result = 34;  // In range
    }
    
    // Multiple range checks
    if (val < 5) {
        result = 35;
    } else if (val < 10) {
        result = 36;
    } else if (val < 15) {
        result = 37;
    } else if (val < 20) {
        result = 38;
    } else {
        result = 39;
    }
    
    // Power of 2 checks
    int num = 16;
    if ((num & (num - 1)) == 0) {
        result = 40;  // Is power of 2
    }
    
    num = 15;
    if ((num & (num - 1)) != 0) {
        result = 41;  // Not power of 2
    }
    
    // Bit field checks
    int flags = 5;  // 0101 in binary
    
    if ((flags & 1) != 0) {  // Bit 0 set
        result = 42;
    }
    
    if ((flags & 2) == 0) {  // Bit 1 not set
        result = 43;
    }
    
    if ((flags & 4) != 0) {  // Bit 2 set
        result = 44;
    }
    
    // Multiple bit checks
    if ((flags & 1) && (flags & 4)) {
        result = 45;  // Bits 0 and 2 set
    }
    
    // Sign checks
    int signed_val = -10;
    if (signed_val < 0) {
        result = 46;  // Negative
    }
    
    signed_val = 10;
    if (signed_val > 0) {
        result = 47;  // Positive
    }
    
    signed_val = 0;
    if (signed_val == 0) {
        result = 48;  // Zero
    }
    
    // Overflow-like scenarios
    int max_like = 2147483647;  // Close to INT_MAX
    if (max_like > 2000000000) {
        result = 49;
    }
    
    // String-like comparisons (char arrays)
    int str1 = 65;  // 'A'
    int str2 = 66;  // 'B'
    if (str1 < str2) {
        result = 50;
    }
    
    // Character range checks
    int ch = 97;  // 'a'
    if (ch >= 97 && ch <= 122) {  // 'a' to 'z'
        result = 51;
    }
    
    ch = 65;  // 'A'
    if (ch >= 65 && ch <= 90) {  // 'A' to 'Z'
        result = 52;
    }
    
    // Digit check
    ch = 48;  // '0'
    if (ch >= 48 && ch <= 57) {  // '0' to '9'
        result = 53;
    }
    
    // Multiple variable updates based on condition
    int x = 10;
    int y = 20;
    int z = 30;
    
    if (x < y && y < z) {
        x = x + 1;
        y = y + 1;
        z = z + 1;
        result = x + y + z;
    }
    
    // Swapping based on condition
    if (x > y) {
        int temp = x;
        x = y;
        y = temp;
    }
    
    if (x > z) {
        int temp = x;
        x = z;
        z = temp;
    }
    
    if (y > z) {
        int temp = y;
        y = z;
        z = temp;
    }
    
    // Now x <= y <= z
    result = x;
    
    // Min/max using if-else
    int min_val = 0;
    int max_val = 0;
    
    if (a < b) {
        min_val = a;
        max_val = b;
    } else {
        min_val = b;
        max_val = a;
    }
    
    // Three-way min
    int p = 5;
    int q = 10;
    int r = 3;
    int min3 = p;
    
    if (q < min3) {
        min3 = q;
    }
    if (r < min3) {
        min3 = r;
    }
    
    result = min3;
    
    // Three-way max
    int max3 = p;
    if (q > max3) {
        max3 = q;
    }
    if (r > max3) {
        max3 = r;
    }
    
    result = max3;
    
    // Absolute value
    int abs_val = -42;
    if (abs_val < 0) {
        abs_val = 0 - abs_val;
    }
    result = abs_val;
    
    // Clamping value to range
    int clamp_val = 25;
    int min_range = 10;
    int max_range = 20;
    
    if (clamp_val < min_range) {
        clamp_val = min_range;
    }
    if (clamp_val > max_range) {
        clamp_val = max_range;
    }
    
    result = clamp_val;
    
    return result;
}
