// Comprehensive test for switch-case statements

int main() {
    int result = 0;
    
    // ========================================
    // TEST 1: Basic switch with cases
    // ========================================
    int x = 2;
    switch (x) {
        case 1:
            result = 10;
            break;
        case 2:
            result = 20;
            break;
        case 3:
            result = 30;
            break;
        default:
            result = 0;
    }
    
    // ========================================
    // TEST 2: Switch without break (fall-through)
    // ========================================
    int y = 1;
    switch (y) {
        case 1:
            result = result + 1;
        case 2:
            result = result + 2;
        case 3:
            result = result + 3;
            break;
        default:
            result = result + 100;
    }
    
    // ========================================
    // TEST 3: Switch with only default
    // ========================================
    int z = 10;
    switch (z) {
        default:
            result = 999;
    }
    
    // ========================================
    // TEST 4: Switch with multiple statements per case
    // ========================================
    int a = 1;
    switch (a) {
        case 1:
            int temp1 = 5;
            int temp2 = 10;
            result = temp1 + temp2;
            break;
        case 2:
            result = 100;
            break;
    }
    
    // ========================================
    // TEST 5: Nested switch
    // ========================================
    int b = 2;
    int c = 1;
    switch (b) {
        case 1:
            result = 111;
            break;
        case 2:
            switch (c) {
                case 1:
                    result = 222;
                    break;
                case 2:
                    result = 333;
                    break;
            }
            break;
        case 3:
            result = 444;
            break;
    }
    
    // ========================================
    // TEST 6: Switch with char
    // ========================================
    char ch = 'a';
    switch (ch) {
        case 'a':
            result = 65;
            break;
        case 'b':
            result = 66;
            break;
        default:
            result = 0;
    }
    
    // ========================================
    // TEST 7: Empty case
    // ========================================
    int d = 5;
    switch (d) {
        case 1:
            break;
        case 2:
            break;
        case 5:
            result = 555;
            break;
        default:
            result = 0;
    }
    
    // ========================================
    // TEST 8: Multiple cases before break
    // ========================================
    int e = 2;
    switch (e) {
        case 1:
        case 2:
        case 3:
            result = 123;
            break;
        case 4:
        case 5:
            result = 45;
            break;
        default:
            result = 0;
    }
    
    // ========================================
    // TEST 9: Switch with if-else inside case
    // ========================================
    int f = 1;
    int g = 5;
    switch (f) {
        case 1:
            if (g > 3) {
                result = 100;
            } else {
                result = 50;
            }
            break;
        case 2:
            result = 200;
            break;
    }
    
    // ========================================
    // TEST 10: Switch with loop inside case
    // ========================================
    int h = 1;
    switch (h) {
        case 1:
            for (int i = 0; i < 3; i++) {
                result = result + i;
            }
            break;
        case 2:
            result = 999;
            break;
    }
    
    // ========================================
    // TEST 11: Switch with arithmetic expression
    // ========================================
    int val = 10;
    switch (val / 2) {
        case 5:
            result = 500;
            break;
        case 10:
            result = 1000;
            break;
        default:
            result = 0;
    }
    
    // ========================================
    // TEST 12: Switch with no default
    // ========================================
    int j = 10;
    switch (j) {
        case 1:
            result = 1;
            break;
        case 2:
            result = 2;
            break;
    }
    
    // ========================================
    // TEST 13: Fall-through to default
    // ========================================
    int k = 1;
    switch (k) {
        case 1:
            result = result + 10;
        default:
            result = result + 5;
    }
    
    // ========================================
    // TEST 14: Switch with break in nested if
    // ========================================
    int m = 1;
    switch (m) {
        case 1:
            if (m == 1) {
                result = 777;
                break;
            }
            result = 888;
            break;
        case 2:
            result = 999;
            break;
    }
    
    // ========================================
    // TEST 15: Large number of cases
    // ========================================
    int n = 5;
    switch (n) {
        case 1:
            result = 1;
            break;
        case 2:
            result = 2;
            break;
        case 3:
            result = 3;
            break;
        case 4:
            result = 4;
            break;
        case 5:
            result = 5;
            break;
        case 6:
            result = 6;
            break;
        case 7:
            result = 7;
            break;
        case 8:
            result = 8;
            break;
        case 9:
            result = 9;
            break;
        case 10:
            result = 10;
            break;
        default:
            result = 0;
    }
    
    return result;
}
