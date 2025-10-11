// Edge case tests for do-while

int main() {
    // Test 1: Empty body
    int x = 0;
    do {
    } while (x++ < 3);
    
    // Test 2: Multiple statements in body
    int a = 0;
    int b = 0;
    do {
        a = a + 1;
        b = b + 2;
        int temp = a + b;
    } while (a < 3);
    
    // Test 3: Do-while with function call
    int result = 0;
    do {
        result = result + 1;
    } while (result < 2);
    
    // Test 4: Complex condition with logical operators
    int m = 0;
    int n = 10;
    do {
        m = m + 1;
        n = n - 1;
    } while (m < 5 || n > 5);
    
    // Test 5: Do-while with ternary in condition
    int val = 0;
    do {
        val = val + 1;
    } while ((val < 3) ? 1 : 0);
    
    // Test 6: Infinite loop with break
    int counter = 0;
    do {
        counter = counter + 1;
        if (counter >= 10) {
            break;
        }
    } while (1);
    
    return x + a + b + result + m + n + val + counter;
}
