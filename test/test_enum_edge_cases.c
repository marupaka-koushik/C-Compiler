// Test 1: Basic enum
enum Color {
    RED,
    GREEN,
    BLUE
};

// Test 2: Enum with explicit values
enum Status {
    SUCCESS = 0,
    ERROR = -1,
    PENDING = 1
};

// Test 3: Anonymous enum
enum {
    NORTH,
    SOUTH,
    EAST,
    WEST
} direction;

// Test 4: Multiple enum variables
enum Day {
    MON,
    TUE,
    WED
};

enum Day today, tomorrow;

int main() {
    enum Color c = RED;
    enum Status s = SUCCESS;
    
    // Enum arithmetic
    int sum = RED + GREEN + BLUE;
    
    // Enum comparisons
    if (c == RED) {
        s = ERROR;
    }
    
    // Enum in expressions
    int result = (c == BLUE) ? 1 : 0;
    
    return sum + result;
}
