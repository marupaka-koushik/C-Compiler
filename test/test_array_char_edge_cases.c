// Edge cases for character arrays - Advanced patterns

int main() {
    // 1. String concatenation (manual)
    char str1[20] = "Hello";
    char str2[20] = "World";
    char result[40];
    int i = 0;
    int j = 0;
    
    while (str1[i] != '\0') {
        result[j] = str1[i];
        i = i + 1;
        j = j + 1;
    }
    result[j] = ' ';
    j = j + 1;
    i = 0;
    while (str2[i] != '\0') {
        result[j] = str2[i];
        i = i + 1;
        j = j + 1;
    }
    result[j] = '\0';
    
    // 2. Character case conversion
    char text[50] = "HeLLo WoRLd";
    for (i = 0; text[i] != '\0'; i = i + 1) {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = text[i] + 32;  // to lowercase
        } else if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] = text[i] - 32;  // to uppercase
        }
    }
    
    // 3. Character array reversal
    char original[20] = "reverse";
    char reversed[20];
    int len = 0;
    while (original[len] != '\0') {
        len = len + 1;
    }
    for (i = 0; i < len; i = i + 1) {
        reversed[i] = original[len - 1 - i];
    }
    reversed[len] = '\0';
    
    // 4. Character counting
    char sentence[100] = "count vowels here";
    int vowel_count = 0;
    for (i = 0; sentence[i] != '\0'; i = i + 1) {
        char ch = sentence[i];
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' ||
            ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U') {
            vowel_count = vowel_count + 1;
        }
    }
    
    // 5. String comparison (lexicographic)
    char s1[20] = "apple";
    char s2[20] = "banana";
    int cmp = 0;
    for (i = 0; s1[i] != '\0' && s2[i] != '\0'; i = i + 1) {
        if (s1[i] < s2[i]) {
            cmp = -1;
            break;
        } else if (s1[i] > s2[i]) {
            cmp = 1;
            break;
        }
    }
    
    // 6. Character array with escape sequences
    char escapes[50];
    escapes[0] = '\n';
    escapes[1] = '\t';
    escapes[2] = '\r';
    escapes[3] = '\\';
    escapes[4] = '\'';
    escapes[5] = '\"';
    escapes[6] = '\0';
    
    // 7. ASCII value operations
    char ascii[10];
    for (i = 0; i < 10; i = i + 1) {
        ascii[i] = '0' + i;  // '0' to '9'
    }
    
    // 8. Character grid/matrix
    char board[3][3];
    board[0][0] = 'X';
    board[0][1] = 'O';
    board[0][2] = 'X';
    board[1][0] = 'O';
    board[1][1] = 'X';
    board[1][2] = 'O';
    board[2][0] = 'X';
    board[2][1] = 'O';
    board[2][2] = 'X';
    
    // 9. String search (find character)
    char haystack[50] = "find the needle";
    char needle = 'n';
    int found_at = -1;
    for (i = 0; haystack[i] != '\0'; i = i + 1) {
        if (haystack[i] == needle) {
            found_at = i;
            break;
        }
    }
    
    // 10. Character replacement
    char replace_in[50] = "hello world";
    char old_char = 'o';
    char new_char = '0';
    for (i = 0; replace_in[i] != '\0'; i = i + 1) {
        if (replace_in[i] == old_char) {
            replace_in[i] = new_char;
        }
    }
    
    // 11. Palindrome check
    char palindrome[20] = "racecar";
    int is_palindrome = 1;
    len = 0;
    while (palindrome[len] != '\0') {
        len = len + 1;
    }
    for (i = 0; i < len / 2; i = i + 1) {
        if (palindrome[i] != palindrome[len - 1 - i]) {
            is_palindrome = 0;
            break;
        }
    }
    
    // 12. Character whitespace handling
    char trim[50] = "  spaces  ";
    int start = 0;
    while (trim[start] == ' ') {
        start = start + 1;
    }
    
    return 0;
}
