// Comprehensive test for scanf - VALID cases

int main() {
    // 1. Basic integer input
    int x;
    scanf("%d", &x);
    
    // 2. Multiple integers
    int a, b;
    scanf("%d %d", &a, &b);
    
    // 3. Character input
    char c;
    scanf("%c", &c);
    
    // 4. Long integer
    long l;
    scanf("%ld", &l);
    
    // 5. Unsigned integer
    unsigned int u;
    scanf("%u", &u);
    
    // 6. Hexadecimal input
    int hex;
    scanf("%x", &hex);
    scanf("%X", &hex);
    
    // 7. Octal input
    int oct;
    scanf("%o", &oct);
    
    // 8. String input
    char str[50];
    scanf("%s", str);
    
    // 9. Multiple format specifiers
    int i1, i2, i3, i4;
    scanf("%d %d %d %d", &i1, &i2, &i3, &i4);
    
    // 10. Array element input
    int arr[5];
    scanf("%d", &arr[0]);
    scanf("%d", &arr[1]);
    scanf("%d", &arr[2]);
    
    // 11. Structure member input
    struct Point {
        int x;
        int y;
    };
    struct Point p;
    scanf("%d %d", &(p.x), &(p.y));
    
    // 12. Scanf in loop
    int numbers[10];
    int i;
    for (i = 0; i < 5; i = i + 1) {
        scanf("%d", &numbers[i]);
    }
    
    // 13. Scanf in conditional
    int choice;
    scanf("%d", &choice);
    if (choice == 1) {
        int val;
        scanf("%d", &val);
    }
    
    // 14. Character and integer
    char ch;
    int num;
    scanf("%c %d", &ch, &num);
    
    // 15. Width specifier for input
    int limited;
    scanf("%3d", &limited);
    
    // 16. Skip whitespace
    char c1, c2;
    scanf(" %c %c", &c1, &c2);
    
    // 17. Pointer to variable
    int value;
    int *ptr = &value;
    scanf("%d", ptr);
    
    // 18. Multiple scanf calls
    int x1, y1, z1;
    scanf("%d", &x1);
    scanf("%d", &y1);
    scanf("%d", &z1);
    
    // 19. Mixed types
    int integer;
    char character;
    long longint;
    scanf("%d %c %ld", &integer, &character, &longint);
    
    // 20. Scanf with array
    char buffer[100];
    scanf("%s", buffer);
    
    return 0;
}
