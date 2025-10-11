// Comparison: do-while vs while loop behavior

int main() {
    // Test 1: Do-while executes at least once
    int do_count = 0;
    do {
        do_count = do_count + 1;
    } while (0);  // False condition, but body executes once
    
    // Test 2: While may not execute at all
    int while_count = 0;
    while (0) {  // False condition, body never executes
        while_count = while_count + 1;
    }
    
    // Test 3: Do-while with initially true condition
    int x = 0;
    do {
        x = x + 1;
    } while (x < 5);
    
    // Test 4: While with initially true condition
    int y = 0;
    while (y < 5) {
        y = y + 1;
    }
    
    // Both should have same result when condition is initially true
    // But do_count=1 and while_count=0 shows the difference
    
    return do_count + while_count + x + y;
}
