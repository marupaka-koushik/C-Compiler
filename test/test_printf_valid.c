// Comprehensive test for printf - VALID cases

int main() {
    // 1. Basic integer printing
    int x = 42;
    printf("%d", x);
    
    // 2. Multiple integers
    int a = 10;
    int b = 20;
    printf("%d %d", a, b);
    
    // 3. Character printing
    char c = 'A';
    printf("%c", c);
    
    // 4. String literal
    printf("Hello, World!");
    
    // 5. Mixed format specifiers
    int num = 100;
    char ch = 'X';
    printf("Number: %d, Char: %c", num, ch);
    
    // 6. Simple integer printing
    int num6 = 100;
    printf("%d", num6);
    
    // 7. Unsigned integer
    unsigned int u;
    u = 100;
    printf("%u", u);
    
    // 8. Hexadecimal
    int hex = 255;
    printf("Hex: %x", hex);
    printf("HEX: %X", hex);
    
    // 9. Octal
    int oct = 64;
    printf("Octal: %o", oct);
    
    // 11. Newline and escape sequences
    printf("Line 1\n");
    printf("Tab\there\n");
    printf("Quote: \"Hello\"\n");
    
    // 12. Multiple format specifiers
    int i1 = 1, i2 = 2, i3 = 3, i4 = 4;
    printf("%d %d %d %d", i1, i2, i3, i4);
    
    // 13. Array element printing
    int arr[5];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    printf("%d %d %d", arr[0], arr[1], arr[2]);
    
    // 14. Expression in printf
    int x1 = 5;
    int y1 = 10;
    printf("Sum: %d", x1 + y1);
    
    // 15. Character array (string)
    char str[20];
    str[0] = 'H';
    str[1] = 'i';
    str[2] = 0;
    printf("%s", str);
    
    // 16. Struct member printing
    struct Point {
        int x;
        int y;
    };
    struct Point p;
    p.x = 100;
    p.y = 200;
    printf("Point: (%d, %d)", p.x, p.y);
    
    // 17. Printf in loop
    int i;
    for (i = 0; i < 5; i = i + 1) {
        printf("%d ", i);
    }
    printf("\n");
    
    // 18. Printf in conditional
    int cond = 1;
    if (cond) {
        printf("Condition is true\n");
    } else {
        printf("Condition is false\n");
    }
    
    // 19. Printf in switch
    int choice = 2;
    switch (choice) {
        case 1:
            printf("Choice 1\n");
            break;
        case 2:
            printf("Choice 2\n");
            break;
        default:
            printf("Default choice\n");
            break;
    }
    
    // 20. Width specifier
    int num2 = 42;
    printf("%5d", num2);
    printf("%10d", num2);
    
    // 21. Padding with zeros
    int num3 = 7;
    printf("%03d", num3);
    printf("%05d", num3);
    
    // 22. Precision for integers
    int num4 = 123;
    printf("%.5d", num4);
    
    // 23. Left-aligned
    int num5 = 99;
    printf("%-10d", num5);
    
    // 24. Sign display
    int pos = 42;
    int neg = -42;
    printf("%+d", pos);
    printf("%+d", neg);
    
    // 25. Space for positive numbers
    printf("% d", pos);
    printf("% d", neg);
    
    return 0;
}
