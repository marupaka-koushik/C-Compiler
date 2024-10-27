// Test 8: I/O Functionality (Simplified without for loop)

int main() {
    int a, arr[3], val, temp, sum;
    int* ptr;

    // Test 1.1: Simple String
    printf("--- TEST START ---\n");
    printf("If you see this, string printing works.\n");

    // Test 1.2: Integer Printing
    a = 100;
    printf("Number: %d\n", a);

    // Test 2.1: Basic Integer Scan
    printf("\n[Input Test 1] Please enter the number 42: ");
    scanf("%d", &a);
    
    if (a != 42) {
        printf("Error: You entered %d instead of 42.\n", a);
        return 49;
    }
    printf("Success! Received 42.\n");

    // Test 2.2: Scanf into Array Element
    arr[2] = 0;
    printf("[Input Test 2] Please enter the number 999: ");
    scanf("%d", &arr[2]);
    
    if (arr[2] != 999) {
        printf("Error: Value in array is %d\n", arr[2]);
        return 65;
    }
    printf("Success! Received 999.\n");

    // Test 2.3: Scanf via Pointer
    val = 0;
    ptr = &val;
    printf("[Input Test 3] Please enter the number -5: ");
    scanf("%d", ptr);
    
    if (*ptr != -5) {
        printf("Error: Value at pointer is %d\n", *ptr);
        return 81;
    }
    if (val != -5) {
        return 85;
    }
    printf("Success! Received -5.\n");

    // Test 4: Multiple scanf (unrolled loop)
    printf("\n[Sum Test] Enter 10, then 20, then 30:\n");
    
    sum = 0;
    
    printf("Enter number 1: ");
    scanf("%d", &temp);
    sum = sum + temp;
    
    printf("Enter number 2: ");
    scanf("%d", &temp);
    sum = sum + temp;
    
    printf("Enter number 3: ");
    scanf("%d", &temp);
    sum = sum + temp;
    
    if (sum != 60) {
        printf("Error: Sum is %d (Expected 60)\n", sum);
        return 119;
    }
    printf("Success! Sum is 60.\n");

    printf("\n--- ALL I/O TESTS PASSED ---\n");
    return 0;
}
