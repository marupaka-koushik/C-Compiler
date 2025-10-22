// Test 1: Basic union
union Data {
    int i;
    float f;
    char c;
};

// Test 2: Anonymous union
union {
    int x;
    int y;
} coord;

// Test 3: Union with different sized members
union Mixed {
    char byte;
    int word;
    float real;
    char buffer[10];
};

// Test 4: Multiple union variables
union Value {
    int asInt;
    float asFloat;
};

union Value v1, v2;

int main() {
    union Data d;
    union Mixed m;
    
    // Union member access would be tested if member access works
    
    // Basic operations
    int x = 10;
    float y = 3.14;
    
    return x;
}
