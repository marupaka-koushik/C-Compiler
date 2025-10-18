// Basic type typedefs
typedef int Integer;
typedef char Character;
typedef float Real;
typedef unsigned int UInt;

// Pointer typedefs
typedef int* IntPtr;
typedef char* String;

// Enum typedef
typedef enum {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY
} Weekday;

// Struct typedef
typedef struct {
    int x;
    int y;
} Point;

// Union typedef
typedef union {
    int asInt;
    float asFloat;
    char asChar;
} Value;

int main() {
    // Basic type usage
    Integer a = 10;
    Character b = 'A';
    Real c = 3.14;
    UInt d = 42;
    
    // Pointer usage
    Integer val = 100;
    IntPtr ptr = &val;
    
    // Enum usage
    Weekday today = MONDAY;
    Weekday tomorrow = TUESDAY;
    
    if (today == MONDAY) {
        tomorrow = FRIDAY;
    }
    
    // Arithmetic with typedefs
    Integer sum = a + d;
    Integer product = sum * 2;
    
    // Expressions
    Integer result = (a > 5) ? (sum + 10) : (product - 5);
    
    // Loops
    Integer i;
    for (i = 0; i < 5; i = i + 1) {
        result = result + i;
    }
    
    while (result > 100) {
        result = result - 10;
    }
    
    return result;
}
