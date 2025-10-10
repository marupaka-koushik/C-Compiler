// Comprehensive test for static keyword
// Tests: static local variables, static global variables, static functions

// Test 1: Static global variable (file scope)
static int global_counter = 0;

// Test 2: Static global array
static int static_array[5] = {1, 2, 3, 4, 5};

// Test 3: Static function (internal linkage)
static int helper_function(int x) {
    return x * 2;
}

// Test 4: Static function with static local variable
static int increment_counter() {
    static int counter = 0;
    counter = counter + 1;
    return counter;
}

// Test 5: Multiple static local variables in same function
static int multi_static_locals() {
    static int a = 10;
    static int b = 20;
    int c = 30;
    
    a = a + 1;
    b = b + 2;
    c = c + 3;
    
    return a + b + c;
}

// Test 6: Static variable in recursive function
static int factorial_with_static(int n) {
    static int call_count = 0;
    call_count = call_count + 1;
    
    if (n <= 1) {
        return 1;
    }
    return n * factorial_with_static(n - 1);
}

// Test 7: Static variable preserving state
int call_counter() {
    static int calls = 0;
    calls = calls + 1;
    return calls;
}

// Test 8: Static array in function
int sum_and_store() {
    static int stored_values[3] = {0, 0, 0};
    static int index = 0;
    
    int sum = stored_values[0] + stored_values[1] + stored_values[2];
    
    if (index < 3) {
        stored_values[index] = index * 10;
        index = index + 1;
    }
    
    return sum;
}

// Test 9: Static pointer variable
int use_static_pointer() {
    static int value = 42;
    static int *ptr = &value;
    
    *ptr = *ptr + 1;
    return *ptr;
}

// Test 10: Static variable in loop
int loop_with_static() {
    static int total = 0;
    int i = 0;
    
    for (i = 0; i < 5; i = i + 1) {
        total = total + i;
    }
    
    return total;
}

// Test 11: Multiple static functions
static int add_static(int a, int b) {
    return a + b;
}

static int multiply_static(int a, int b) {
    return a * b;
}

static int compute(int x, int y) {
    int sum = add_static(x, y);
    int product = multiply_static(x, y);
    return sum + product;
}

// Test 12: Static variable with different scopes
int nested_scopes() {
    static int outer = 100;
    outer = outer + 1;
    
    if (outer > 100) {
        static int inner = 50;
        inner = inner + 1;
        return inner;
    }
    
    return outer;
}

// Test 13: Static char and string operations
int static_char_test() {
    static char c = 'A';
    static int ascii = 65;
    
    c = c + 1;
    ascii = ascii + 1;
    
    return ascii;
}

// Test 14: Static in switch statement
int switch_with_static(int choice) {
    static int case_counts[4] = {0, 0, 0, 0};
    
    switch (choice) {
        case 0:
            case_counts[0] = case_counts[0] + 1;
            break;
        case 1:
            case_counts[1] = case_counts[1] + 1;
            break;
        case 2:
            case_counts[2] = case_counts[2] + 1;
            break;
        default:
            case_counts[3] = case_counts[3] + 1;
            break;
    }
    
    return case_counts[choice];
}

// Test 15: Static in while loop
int while_with_static() {
    static int iterations = 0;
    int count = 0;
    
    while (count < 3) {
        iterations = iterations + 1;
        count = count + 1;
    }
    
    return iterations;
}

// Test 16: Static in do-while loop
int do_while_with_static() {
    static int sum = 0;
    int i = 0;
    
    do {
        sum = sum + i;
        i = i + 1;
    } while (i < 3);
    
    return sum;
}

// Test 17: Static with conditional expressions
int conditional_static(int flag) {
    static int true_count = 0;
    static int false_count = 0;
    
    if (flag) {
        true_count = true_count + 1;
        return true_count;
    } else {
        false_count = false_count + 1;
        return false_count;
    }
}

// Test 18: Static variable initialization only once
int initialization_test() {
    static int x = 100;  // Initialized only on first call
    x = x + 1;
    return x;
}

// Test 19: Static with complex expressions
static int complex_static_function(int a, int b) {
    static int result = 0;
    
    result = (a + b) * (a - b);
    
    if (result > 0) {
        result = result * 2;
    } else {
        result = result * -1;
    }
    
    return result;
}

// Test 20: Static with nested function calls
static int inner_static(int x) {
    static int base = 10;
    base = base + x;
    return base;
}

int outer_with_static(int val) {
    static int outer_val = 5;
    outer_val = outer_val + inner_static(val);
    return outer_val;
}

// Test 21: Static global with function using it
static int global_static_var = 999;

int use_global_static() {
    global_static_var = global_static_var + 1;
    return global_static_var;
}

// Test 22: Multiple static declarations in sequence
int multiple_static_sequence() {
    static int a = 1;
    static int b = 2;
    static int c = 3;
    static int d = 4;
    
    a = a + b;
    c = c + d;
    
    return a + c;
}

// Test 23: Static with goto
int static_with_goto(int x) {
    static int goto_count = 0;
    
    if (x > 0) {
        goto_count = goto_count + 10;
    } else {
        goto_count = goto_count + 1;
    }
    
    return goto_count;
}

// Test 24: Static in nested blocks
int nested_block_static() {
    static int level1 = 10;
    level1 = level1 + 1;
    
    {
        static int level2 = 20;
        level2 = level2 + 2;
        
        {
            static int level3 = 30;
            level3 = level3 + 3;
            return level1 + level2 + level3;
        }
    }
}

// Test 25: Static with all operators
int static_all_operators() {
    static int val = 100;
    
    val = val + 10;
    val = val - 5;
    val = val * 2;
    val = val / 3;
    val = val % 7;
    
    if (val > 50) {
        val = val & 127;
    }
    
    if (val < 200) {
        val = val | 8;
    }
    
    val = val ^ 15;
    val = val << 1;
    val = val >> 1;
    
    return val;
}

int main() {
    int result = 0;
    int i = 0;
    
    // Test static local variables
    result = call_counter();
    result = call_counter();
    result = call_counter();
    
    // Test static functions
    result = helper_function(5);
    result = increment_counter();
    
    // Test static in various control structures
    result = loop_with_static();
    result = while_with_static();
    result = do_while_with_static();
    
    // Test static with recursion
    result = factorial_with_static(5);
    
    // Test initialization
    result = initialization_test();
    result = initialization_test();
    
    // Test multiple calls
    for (i = 0; i < 3; i = i + 1) {
        result = sum_and_store();
    }
    
    // Test static global
    result = use_global_static();
    
    // Test complex scenarios
    result = nested_block_static();
    result = static_all_operators();
    
    return 0;
}
