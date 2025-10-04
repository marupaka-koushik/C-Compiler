// Comprehensive test for character arrays (strings) - VALID cases

int main() {
    // 1. Basic character array declaration and initialization
    char str1[10];
    char str2[6] = "hello";
    char str3[] = "world";
    char str4[5] = {'a', 'b', 'c', 'd', '\0'};
    
    // 2. Individual character access and assignment
    str1[0] = 'H';
    str1[1] = 'i';
    str1[2] = '\0';
    char ch = str2[0];
    
    // 3. Character array manipulation
    char name[20];
    name[0] = 'J';
    name[1] = 'o';
    name[2] = 'h';
    name[3] = 'n';
    name[4] = '\0';
    
    // 4. Array of characters with special characters
    char special[10] = "a\nb\tc";  // newline, tab
    
    // 5. Character array in loops
    int i;
    char alphabet[26];
    for (i = 0; i < 26; i = i + 1) {
        alphabet[i] = 'a' + i;
    }
    
    // 6. Character comparison
    if (str2[0] == 'h') {
        ch = 'H';
    }
    
    // 7. Character array element in switch
    switch(str2[0]) {
        case 'h':
            ch = 'H';
            break;
        case 'w':
            ch = 'W';
            break;
        default:
            ch = '?';
            break;
    }
    
    // 8. Multi-dimensional character arrays
    char grid[3][3];
    grid[0][0] = 'X';
    grid[0][1] = 'O';
    grid[1][0] = 'X';
    
    // 9. String array (array of strings)
    char words[3][10];
    words[0][0] = 'c';
    words[0][1] = 'a';
    words[0][2] = 't';
    words[0][3] = '\0';
    
    // 10. Character arithmetic
    char c1 = 'A';
    char c2 = c1 + 1;  // 'B'
    char c3 = c1 + 32; // 'a' (lowercase)
    
    // 11. Character array with escape sequences
    char escapes[10] = "\"\'\\n\\t";
    
    // 12. Empty string
    char empty[1] = "";
    
    // 13. Character array modification
    str1[0] = str2[0];
    str1[1] = str2[1];
    
    // 14. Character increment/decrement
    str1[0]++;
    str1[1]--;
    
    // 15. Character in expressions
    int ascii = str2[0];  // Get ASCII value
    char upper = str2[0] - 32;
    
    // 16. Character array length calculation (manual)
    int len = 0;
    for (i = 0; str2[i] != '\0'; i = i + 1) {
        len = len + 1;
    }
    
    // 17. Character array comparison (manual)
    int same = 1;
    for (i = 0; str2[i] != '\0' && str3[i] != '\0'; i = i + 1) {
        if (str2[i] != str3[i]) {
            same = 0;
            break;
        }
    }
    
    // 18. Character array copy (manual)
    char copy[20];
    for (i = 0; str2[i] != '\0'; i = i + 1) {
        copy[i] = str2[i];
    }
    copy[i] = '\0';
    
    // 19. Character range check
    if (str2[0] >= 'a' && str2[0] <= 'z') {
        ch = str2[0] - 32;  // Convert to uppercase
    }
    
    // 20. Character in ternary
    char result = (str2[0] == 'h') ? 'Y' : 'N';
    
    // 21. Null character handling
    char null_char = '\0';
    str1[5] = null_char;
    
    // 22. Character digits
    char digits[11] = "0123456789";
    
    // 23. Mixed initialization
    char mixed[10] = {'h', 'e', 'l', 'l', 'o'};
    
    // 24. Large character array
    char buffer[256];
    buffer[0] = 's';
    buffer[255] = '\0';
    
    // 25. Character constant in array
    char constants[5] = {65, 66, 67, 68, 0};  // 'A', 'B', 'C', 'D', '\0'
    
    return 0;
}
