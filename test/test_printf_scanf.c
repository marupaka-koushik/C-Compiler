// Comprehensive printf and scanf testing

int main() {
    // Test 1: Simple printf with string literal
    printf("Hello World\n");
    
    // Test 2: printf with integer format specifier
    int a = 42;
    printf("%d\n", a);
    
    // Test 3: printf with multiple integers
    int x = 10;
    int y = 20;
    printf("%d %d\n", x, y);
    
    // Test 4: printf with character
    char c = 'A';
    printf("%c\n", c);
    
    // Test 5: printf with string and integer
    printf("Value: %d\n", 100);
    
    // Test 6: printf with expression
    int num1 = 5;
    int num2 = 3;
    printf("Sum: %d\n", num1 + num2);
    
    // Test 7: Multiple printf statements
    printf("Line 1\n");
    printf("Line 2\n");
    printf("Line 3\n");
    
    // Test 8: scanf for integer
    int input1;
    scanf("%d", &input1);
    
    // Test 9: scanf for character
    char ch;
    scanf("%c", &ch);
    
    // Test 10: scanf for multiple integers
    int val1;
    int val2;
    scanf("%d %d", &val1, &val2);
    
    // Test 11: scanf followed by printf
    int number;
    printf("Enter a number: ");
    scanf("%d", &number);
    printf("You entered: %d\n", number);
    
    // Test 12: printf with multiple format specifiers
    int age = 25;
    char grade = 'A';
    printf("Age: %d, Grade: %c\n", age, grade);
    
    // Test 13: scanf and printf in loop
    int i;
    for (i = 0; i < 3; i = i + 1) {
        int temp;
        printf("Enter value %d: ", i);
        scanf("%d", &temp);
        printf("Got: %d\n", temp);
    }
    
    // Test 14: printf with array element
    int arr[5];
    arr[0] = 100;
    arr[1] = 200;
    printf("arr[0] = %d\n", arr[0]);
    printf("arr[1] = %d\n", arr[1]);
    
    // Test 15: scanf into array
    int nums[3];
    scanf("%d", &nums[0]);
    scanf("%d", &nums[1]);
    scanf("%d", &nums[2]);
    
    // Test 16: printf with pointer dereference
    int value = 99;
    int *ptr = &value;
    printf("Value: %d\n", *ptr);
    
    // Test 17: scanf with expressions in printf
    int base = 10;
    printf("Double: %d\n", base * 2);
    printf("Triple: %d\n", base * 3);
    
    // Test 18: Nested loop with printf
    int row;
    int col;
    for (row = 0; row < 2; row = row + 1) {
        for (col = 0; col < 3; col = col + 1) {
            printf("%d,%d ", row, col);
        }
        printf("\n");
    }
    
    // Test 19: printf and scanf in if-else
    int score;
    scanf("%d", &score);
    if (score >= 90) {
        printf("Grade: A\n");
    } else if (score >= 80) {
        printf("Grade: B\n");
    } else {
        printf("Grade: C\n");
    }
    
    // Test 20: printf with while loop
    int count = 5;
    while (count > 0) {
        printf("Count: %d\n", count);
        count = count - 1;
    }
    
    // Test 21: Multiple scanf in sequence
    int p;
    int q;
    int r;
    scanf("%d", &p);
    scanf("%d", &q);
    scanf("%d", &r);
    printf("Sum: %d\n", p + q + r);
    
    // Test 22: printf with complex expressions
    int m = 7;
    int n = 3;
    printf("Addition: %d\n", m + n);
    printf("Subtraction: %d\n", m - n);
    printf("Multiplication: %d\n", m * n);
    printf("Division: %d\n", m / n);
    
    // Test 23: scanf and computation
    int width;
    int height;
    scanf("%d", &width);
    scanf("%d", &height);
    int area = width * height;
    printf("Area: %d\n", area);
    
    // Test 24: printf in different scopes
    {
        int local = 42;
        printf("Local: %d\n", local);
    }
    
    // Test 25: Mixed printf formats
    int id = 101;
    char symbol = '#';
    printf("ID: %d, Symbol: %c\n", id, symbol);
    
    return 0;
}
