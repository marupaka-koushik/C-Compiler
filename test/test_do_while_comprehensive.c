// Comprehensive do-while loop tests

int main() {
    // Test 1: Simple increment
    int i = 0;
    do {
        i = i + 1;
    } while (i < 3);
    
    // Test 2: Do-while with complex condition
    int x = 10;
    int y = 5;
    do {
        x = x - 1;
        y = y + 1;
    } while (x > y);
    
    // Test 3: Do-while with break
    int count = 0;
    do {
        count = count + 1;
        if (count == 5) {
            break;
        }
    } while (count < 10);
    
    // Test 4: Do-while with continue
    int sum = 0;
    int n = 0;
    do {
        n = n + 1;
        if (n == 3) {
            continue;
        }
        sum = sum + n;
    } while (n < 5);
    
    // Test 5: Nested do-while
    int outer = 0;
    int total = 0;
    do {
        int inner = 0;
        do {
            total = total + 1;
            inner = inner + 1;
        } while (inner < 2);
        outer = outer + 1;
    } while (outer < 3);
    
    // Test 6: Do-while with complex expression
    int a = 1;
    do {
        a = a * 2;
    } while (a < 100 && a > 0);
    
    // Test 7: Single iteration (condition false after first)
    int once = 0;
    do {
        once = once + 1;
    } while (0);
    
    return i + x + count + sum + total + a + once;
}
