// Complete typedef test suite combining all features

// Basic typedefs
typedef int Int;
typedef unsigned int UInt;
typedef char Char;
typedef float Float;

// Pointer typedefs
typedef int* IntPtr;
typedef char* String;

// Enum typedef
typedef enum {
    STATE_INIT = 0,
    STATE_RUNNING = 1,
    STATE_STOPPED = 2,
    STATE_ERROR = -1
} State;

// Struct typedef
typedef struct {
    int width;
    int height;
} Rectangle;

// Union typedef
typedef union {
    int asInt;
    float asFloat;
    char bytes[4];
} Number;

int main() {
    // Basic type usage
    Int x = 42;
    UInt y = 100;
    Char c = 'Z';
    Float f = 2.718;
    
    // Pointer usage
    Int value = 999;
    IntPtr ptr = &value;
    
    // Enum usage
    State currentState = STATE_INIT;
    State nextState = STATE_RUNNING;
    
    // Complex expressions with typedefs
    Int sum = x + y;
    Int diff = (x > y) ? (x - y) : (y - x);
    
    // Control flow with enum
    if (currentState == STATE_INIT) {
        nextState = STATE_RUNNING;
    } else if (currentState == STATE_ERROR) {
        nextState = STATE_STOPPED;
    }
    
    // Loops with typedef
    Int i;
    Int total = 0;
    for (i = 0; i < 10; i = i + 1) {
        total = total + i;
    }
    
    while (total > 50) {
        total = total - 5;
    }
    
    // Arithmetic with mixed typedefs
    Int result = sum + diff + total;
    
    // Bitwise operations
    UInt mask = 0xFF;
    UInt masked = result & mask;
    
    // Ternary with typedef
    Int final = (masked > 100) ? 100 : masked;
    
    return final;
}
