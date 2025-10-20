// Comprehensive test for bitwise operators

int main() {
    int a = 60;     // 0011 1100 in binary
    int b = 13;     // 0000 1101 in binary
    int result;
    
    // Bitwise AND (&)
    // 60 = 0011 1100
    // 13 = 0000 1101
    // &  = 0000 1100 = 12
    result = a & b;             // 12
    result = 60 & 13;           // 12
    result = 15 & 7;            // 7 (0000 1111 & 0000 0111 = 0000 0111)
    result = 255 & 15;          // 15 (get lower 4 bits)
    result = a & 0;             // 0 (anything & 0 = 0)
    result = a & -1;            // 60 (anything & all-1s = itself)
    
    // Bitwise OR (|)
    // 60 = 0011 1100
    // 13 = 0000 1101
    // |  = 0011 1101 = 61
    result = a | b;             // 61
    result = 60 | 13;           // 61
    result = 8 | 4;             // 12 (0000 1000 | 0000 0100 = 0000 1100)
    result = 15 | 240;          // 255 (0000 1111 | 1111 0000 = 1111 1111)
    result = a | 0;             // 60 (anything | 0 = itself)
    
    // Bitwise XOR (^)
    // 60 = 0011 1100
    // 13 = 0000 1101
    // ^  = 0011 0001 = 49
    result = a ^ b;             // 49
    result = 60 ^ 13;           // 49
    result = 15 ^ 15;           // 0 (same values XOR to 0)
    result = a ^ 0;             // 60 (anything ^ 0 = itself)
    result = 5 ^ 3;             // 6 (0101 ^ 0011 = 0110)
    
    // Bitwise NOT (~)
    // ~60 = ~0011 1100 = 1100 0011 = -61 (two's complement)
    result = ~a;                // -61
    result = ~0;                // -1 (all bits set)
    result = ~-1;               // 0 (all bits cleared)
    result = ~13;               // -14
    result = ~~a;               // 60 (double NOT returns original)
    
    // Left shift (<<)
    result = 1 << 0;            // 1 (no shift)
    result = 1 << 1;            // 2
    result = 1 << 2;            // 4
    result = 1 << 3;            // 8
    result = 1 << 4;            // 16
    result = 2 << 1;            // 4 (multiply by 2)
    result = 2 << 2;            // 8 (multiply by 4)
    result = 5 << 1;            // 10
    result = 60 << 2;           // 240 (shift left by 2)
    result = a << 1;            // 120
    result = a << 0;            // 60 (no change)
    
    // Right shift (>>)
    result = 8 >> 0;            // 8 (no shift)
    result = 8 >> 1;            // 4
    result = 8 >> 2;            // 2
    result = 8 >> 3;            // 1
    result = 8 >> 4;            // 0
    result = 16 >> 1;           // 8 (divide by 2)
    result = 16 >> 2;           // 4 (divide by 4)
    result = 60 >> 2;           // 15
    result = a >> 1;            // 30
    result = a >> 0;            // 60 (no change)
    
    // Combining bitwise operators
    result = (a & b) | 1;       // 13 (12 | 1)
    result = (a | b) & 15;      // 13 (61 & 15)
    result = (a ^ b) & 255;     // 49
    result = ~(a & b);          // -13 (~12)
    result = ~(a | b);          // -62 (~61)
    
    // Bitwise operators in expressions
    result = (a & 15) + (b & 15);       // 12 + 13 = 25
    result = (a | b) - (a & b);         // 61 - 12 = 49
    result = (a << 1) + (b >> 1);       // 120 + 6 = 126
    
    // Using shift for multiplication/division by powers of 2
    int num = 10;
    result = num << 1;          // 20 (multiply by 2)
    result = num << 2;          // 40 (multiply by 4)
    result = num << 3;          // 80 (multiply by 8)
    result = num >> 1;          // 5 (divide by 2)
    result = num >> 2;          // 2 (divide by 4)
    
    // Bit manipulation patterns
    int flags = 0;
    flags = flags | 1;          // Set bit 0
    flags = flags | 2;          // Set bit 1
    flags = flags | 4;          // Set bit 2
    result = flags & 1;         // Check bit 0
    result = flags & 2;         // Check bit 1
    flags = flags & ~1;         // Clear bit 0
    flags = flags ^ 2;          // Toggle bit 1
    
    // Masking operations
    int value = 0x12345678;     // Some hex value
    result = value & 0xFF;      // Get lowest byte
    result = (value >> 8) & 0xFF;   // Get second byte
    result = value & 0xFFFF;    // Get lowest 2 bytes
    
    // Setting/clearing/toggling bits
    int x = 8;                  // 0000 1000
    x = x | 4;                  // Set bit 2: 0000 1100 = 12
    x = x & ~2;                 // Clear bit 1: 0000 1100 = 12
    x = x ^ 8;                  // Toggle bit 3: 0000 0100 = 4
    
    // Power of 2 checks using bitwise AND
    int p = 16;
    result = p & (p - 1);       // 0 if p is power of 2
    p = 15;
    result = p & (p - 1);       // non-zero if p is not power of 2
    
    // Swap using XOR (classic trick)
    int m = 5;
    int n = 9;
    m = m ^ n;
    n = m ^ n;                  // n becomes 5
    m = m ^ n;                  // m becomes 9
    
    // Bitwise operators with negative numbers
    int neg = -10;
    result = neg & 15;          // Get lower bits
    result = neg | 1;           // Set bit 0
    result = neg << 1;          // Shift negative
    result = neg >> 1;          // Arithmetic shift (sign extension)
    
    // Precedence tests
    result = 1 | 2 & 4;         // 1 | (2 & 4) = 1
    result = 1 ^ 2 | 4;         // (1 ^ 2) | 4 = 7
    result = 8 >> 1 | 4;        // (8 >> 1) | 4 = 4
    result = 2 << 1 & 6;        // (2 << 1) & 6 = 4
    
    return 0;
}
