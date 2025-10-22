// Comprehensive union testing

// Test 1: Simple union declaration
union Data {
    int i;
    float f;
    char c;
};

// Test 2: Union with different types
union Value {
    int num;
    char ch;
};

int main() {
    // Test 3: Declare union variable
    union Data d;
    
    // Test 4: Assign to union members
    d.i = 42;
    int val1 = d.i;
    
    // Test 5: Access different member
    d.c = 'A';
    char val2 = d.c;
    
    // Test 6: Multiple union variables
    union Value v1;
    union Value v2;
    v1.num = 100;
    v2.ch = 'B';
    
    // Test 7: Union in condition
    if (d.i == 42) {
        int check = 1;
    }
    
    return 0;
}
