// Advanced until-loop testing with break, continue, and nested loops

int main() {
    // Test 1: until with break
    int count = 0;
    until (count >= 10) {
        count = count + 1;
        if (count == 5) {
            break;
        }
    }
    
    // Test 2: until with continue
    int sum = 0;
    int n = 0;
    until (n >= 10) {
        n = n + 1;
        if (n == 5) {
            continue;
        }
        sum = sum + n;
    }
    
    // Test 3: nested until loops
    int i = 0;
    until (i >= 3) {
        int j = 0;
        until (j >= 3) {
            j = j + 1;
        }
        i = i + 1;
    }
    
    // Test 4: until with multiple conditions
    int x = 0;
    int y = 10;
    until (x >= 5 && y <= 5) {
        x = x + 1;
        y = y - 1;
    }
    
    return 0;
}
