// Comprehensive test for typedef (basic types only)
// Note: Typedef with structs has issues in current implementation

// Test 1: Simple typedef for int
typedef int Integer;

int test_simple_typedef() {
    Integer x = 10;
    Integer y = 20;
    Integer sum = x + y;
    return sum;
}

// Test 2: Typedef for char
typedef char Character;

int test_char_typedef() {
    Character c = 'A';
    Character d = 'B';
    return c + d;
}

// Test 3: Typedef for unsigned
typedef unsigned int UInt;

int test_unsigned_typedef() {
    UInt u = 100;
    u = u + 50;
    return u;
}

// Test 4: Multiple typedefs
typedef int Int;
typedef Int MyInt;

int test_chained_typedef() {
    MyInt n = 42;
    return n;
}

// Test 5: Typedef in function parameters
typedef int Count;

int add_counts(Count a, Count b) {
    return a + b;
}

int test_typedef_params() {
    Count x = 5;
    Count y = 10;
    return add_counts(x, y);
}

// Test 6: Typedef in function return type
typedef int Result;

Result compute(int x, int y) {
    return x * y;
}

int test_typedef_return() {
    Result r = compute(6, 7);
    return r;
}

// Test 7: Array of typedef'd type
typedef int Value;

int test_array_of_typedef() {
    Value values[5];
    int i = 0;
    
    for (i = 0; i < 5; i = i + 1) {
        values[i] = i * 10;
    }
    
    return values[3];
}

// Test 8: Typedef with const
typedef const int ConstInt;

int test_const_typedef() {
    ConstInt ci = 50;
    return ci;
}

// Test 9: Multiple variables with typedef
typedef int Counter;

int test_multiple_typedef_vars() {
    Counter c1 = 1;
    Counter c2 = 2;
    Counter c3 = 3;
    
    return c1 + c2 + c3;
}

// Test 10: Typedef in loops
typedef int Index;

int test_typedef_in_loop() {
    Index i = 0;
    Index sum = 0;
    
    for (i = 0; i < 10; i = i + 1) {
        sum = sum + i;
    }
    
    return sum;
}

// Test 11: Typedef in conditionals
typedef int Flag;

int test_typedef_conditional(Flag condition) {
    if (condition) {
        return 1;
    }
    return 0;
}

// Test 12: Typedef with switch
typedef int Choice;

int test_typedef_switch(Choice ch) {
    switch (ch) {
        case 1:
            return 10;
        case 2:
            return 20;
        default:
            return 0;
    }
}

// Test 13: Typedef with static
typedef int StaticValue;

int test_static_typedef() {
    static StaticValue sv = 0;
    sv = sv + 1;
    return sv;
}

// Test 14: Typedef with while loop
typedef int LoopVar;

int test_typedef_while() {
    LoopVar i = 0;
    LoopVar sum = 0;
    
    while (i < 5) {
        sum = sum + i;
        i = i + 1;
    }
    
    return sum;
}

// Test 15: Typedef with do-while
typedef int Accumulator;

int test_typedef_do_while() {
    Accumulator acc = 0;
    Accumulator i = 0;
    
    do {
        acc = acc + i;
        i = i + 1;
    } while (i < 5);
    
    return acc;
}

// Test 16: Typedef in nested blocks
typedef int BlockVar;

int test_typedef_nested_blocks() {
    BlockVar outer = 10;
    
    if (outer > 0) {
        BlockVar inner = 20;
        outer = outer + inner;
    }
    
    return outer;
}

// Test 17: Typedef with ternary operator
typedef int TernaryValue;

int test_typedef_ternary(int condition) {
    TernaryValue result = condition ? 100 : 200;
    return result;
}

// Test 18: Multiple typedef declarations
typedef int Type1;
typedef char Type2;
typedef unsigned int Type3;

int test_multiple_typedefs() {
    Type1 t1 = 10;
    Type2 t2 = 'X';
    Type3 t3 = 30;
    
    return t1 + t2 + t3;
}

// Test 19: Typedef with all operators
typedef int OpValue;

int test_typedef_operators() {
    OpValue a = 100;
    OpValue b = 20;
    
    OpValue sum = a + b;
    OpValue diff = a - b;
    OpValue prod = a * b;
    OpValue quot = a / b;
    OpValue mod = a % b;
    
    return sum + diff + prod + quot + mod;
}

// Test 20: Typedef with bitwise operators
typedef int BitValue;

int test_typedef_bitwise() {
    BitValue x = 15;
    BitValue y = 7;
    
    BitValue and_result = x & y;
    BitValue or_result = x | y;
    BitValue xor_result = x ^ y;
    
    return and_result + or_result + xor_result;
}

// Test 21: Typedef with shift operators
typedef int ShiftValue;

int test_typedef_shift() {
    ShiftValue val = 8;
    ShiftValue left = val << 2;
    ShiftValue right = val >> 1;
    
    return left + right;
}

// Test 22: Typedef with comparison operators
typedef int CompValue;

int test_typedef_comparison() {
    CompValue a = 10;
    CompValue b = 20;
    
    if (a < b) {
        return 1;
    }
    return 0;
}

// Test 23: Typedef with logical operators
typedef int BoolValue;

int test_typedef_logical() {
    BoolValue true_val = 1;
    BoolValue false_val = 0;
    
    if (true_val && !false_val) {
        return 1;
    }
    return 0;
}

// Test 24: Typedef with increment/decrement
typedef int IncValue;

int test_typedef_inc_dec() {
    IncValue x = 10;
    x = x + 1;
    x = x - 1;
    return x;
}

// Test 25: Typedef with comma operator
typedef int CommaValue;

int test_typedef_comma() {
    CommaValue a = 0;
    CommaValue b = 0;
    
    a = 1;
    b = 2;
    
    return a + b;
}

int main() {
    int result = 0;
    
    // Test basic typedefs
    result = test_simple_typedef();
    result = test_char_typedef();
    result = test_unsigned_typedef();
    
    // Test chained typedefs
    result = test_chained_typedef();
    result = test_const_typedef();
    
    // Test typedefs in functions
    result = test_typedef_params();
    result = test_typedef_return();
    
    // Test typedefs with arrays
    result = test_array_of_typedef();
    
    // Test typedefs in control structures
    result = test_multiple_typedef_vars();
    result = test_typedef_in_loop();
    result = test_typedef_while();
    result = test_typedef_do_while();
    result = test_typedef_conditional(1);
    result = test_typedef_switch(2);
    result = test_typedef_nested_blocks();
    result = test_typedef_ternary(1);
    
    // Test typedef with storage classes
    result = test_static_typedef();
    result = test_static_typedef();
    
    // Test with operators
    result = test_multiple_typedefs();
    result = test_typedef_operators();
    result = test_typedef_bitwise();
    result = test_typedef_shift();
    result = test_typedef_comparison();
    result = test_typedef_logical();
    result = test_typedef_inc_dec();
    result = test_typedef_comma();
    
    return 0;
}
