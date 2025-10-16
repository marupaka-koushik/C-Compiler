// Comprehensive test for switch-case statements - VALID cases

int main() {
    int x = 2;
    int result = 0;
    
    // Basic switch with break
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
            break;
    }
    
    // Switch without default
    switch (x) {
        case 1:
            result = 1;
            break;
        case 2:
            result = 2;
            break;
        case 3:
            result = 3;
            break;
    }
    
    // Switch with default only
    switch (x) {
        default:
            result = 100;
            break;
    }
    
    // Fall-through behavior (no break)
    switch (x) {
        case 1:
            result = result + 1;
        case 2:
            result = result + 2;
        case 3:
            result = result + 3;
            break;
        default:
            result = result + 100;
            break;
    }
    
    // Multiple cases for same action
    switch (x) {
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
            break;
    }
    
    // Switch with 0 case
    switch (x) {
        case 0:
            result = 0;
            break;
        case 1:
            result = 1;
            break;
        default:
            result = -1;
            break;
    }
    
    // Switch with negative cases
    switch (x) {
        case -1:
            result = -10;
            break;
        case 0:
            result = 0;
            break;
        case 1:
            result = 10;
            break;
        default:
            result = 99;
            break;
    }
    
    // Switch with large numbers
    switch (x) {
        case 100:
            result = 1;
            break;
        case 1000:
            result = 2;
            break;
        case 10000:
            result = 3;
            break;
        default:
            result = 0;
            break;
    }
    
    // Switch with character (treated as int)
    int ch = 97;  // 'a'
    switch (ch) {
        case 97:  // 'a'
            result = 1;
            break;
        case 98:  // 'b'
            result = 2;
            break;
        case 99:  // 'c'
            result = 3;
            break;
        default:
            result = 0;
            break;
    }
    
    // Switch with enum-like values
    int RED = 0;
    int GREEN = 1;
    int BLUE = 2;
    int color = GREEN;
    
    switch (color) {
        case 0:  // RED
            result = 10;
            break;
        case 1:  // GREEN
            result = 20;
            break;
        case 2:  // BLUE
            result = 30;
            break;
        default:
            result = -1;
            break;
    }
    
    // Nested switch statements
    int a = 1;
    int b = 2;
    switch (a) {
        case 1:
            switch (b) {
                case 1:
                    result = 11;
                    break;
                case 2:
                    result = 12;
                    break;
                default:
                    result = 10;
                    break;
            }
            break;
        case 2:
            result = 20;
            break;
        default:
            result = 0;
            break;
    }
    
    // Switch in loop
    int i;
    for (i = 0; i < 5; i = i + 1) {
        switch (i) {
            case 0:
                result = result + 1;
                break;
            case 1:
                result = result + 2;
                break;
            case 2:
                result = result + 3;
                break;
            case 3:
                result = result + 4;
                break;
            case 4:
                result = result + 5;
                break;
        }
    }
    
    // Switch with expression
    switch (x + 1) {
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
            break;
    }
    
    // Switch with multiplication expression
    switch (x * 2) {
        case 2:
            result = 1;
            break;
        case 4:
            result = 2;
            break;
        case 6:
            result = 3;
            break;
        default:
            result = 0;
            break;
    }
    
    // Switch with modulo expression
    switch (x % 3) {
        case 0:
            result = 100;
            break;
        case 1:
            result = 200;
            break;
        case 2:
            result = 300;
            break;
    }
    
    // Empty switch body
    switch (x) {
    }
    
    // Switch with single case
    switch (x) {
        case 2:
            result = 42;
            break;
    }
    
    // Switch with many cases
    switch (x) {
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
            break;
    }
    
    // Switch with multiple statements per case
    switch (x) {
        case 1:
            result = 10;
            result = result + 5;
            result = result * 2;
            break;
        case 2:
            result = 20;
            result = result - 5;
            result = result / 3;
            break;
        default:
            result = 0;
            result = result + 100;
            break;
    }
    
    // Default at beginning (unusual but valid)
    switch (x) {
        default:
            result = 999;
            break;
        case 1:
            result = 1;
            break;
        case 2:
            result = 2;
            break;
    }
    
    // Default in middle (unusual but valid)
    switch (x) {
        case 1:
            result = 1;
            break;
        default:
            result = 999;
            break;
        case 2:
            result = 2;
            break;
    }
    
    // Cases in descending order
    switch (x) {
        case 10:
            result = 1;
            break;
        case 5:
            result = 2;
            break;
        case 1:
            result = 3;
            break;
        default:
            result = 0;
            break;
    }
    
    // Switch with break in middle of case
    switch (x) {
        case 1:
            result = 10;
            if (x > 0) {
                break;
            }
            result = 20;  // Unreachable if x > 0
            break;
        case 2:
            result = 30;
            break;
    }
    
    // Switch with variable declaration in case (valid in C)
    switch (x) {
        case 1: {
            int temp = 100;
            result = temp;
            break;
        }
        case 2: {
            int temp = 200;
            result = temp;
            break;
        }
        default: {
            int temp = 0;
            result = temp;
            break;
        }
    }
    
    return result;
}
