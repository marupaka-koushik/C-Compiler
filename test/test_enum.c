// Comprehensive enum testing

// Test 1: Simple enum declaration
enum Color {
    RED,
    GREEN,
    BLUE
};

// Test 2: Enum with explicit values
enum Status {
    INACTIVE = 0,
    ACTIVE = 1,
    PENDING = 2
};

// Test 3: Enum with mixed assignments
enum Level {
    LOW = 10,
    MEDIUM,
    HIGH
};

int main() {
    // Test 4: Declare enum variable
    enum Color c;
    c = RED;
    
    // Test 5: Use enum in assignment
    enum Status s = ACTIVE;
    
    // Test 6: Use enum in condition
    if (c == RED) {
        int isRed = 1;
    }
    
    // Test 7: Enum in switch
    int val;
    if (s == ACTIVE) {
        val = 1;
    } else if (s == INACTIVE) {
        val = 0;
    }
    
    // Test 8: Enum values as integers
    int colorValue = RED;
    int statusValue = ACTIVE;
    
    // Test 9: Arithmetic with enum
    int sum = LOW + MEDIUM;
    
    // Test 10: Enum in expression
    int result = (c == BLUE) ? 100 : 200;
    
    return 0;
}
