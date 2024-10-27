// Exhaustive Test Suite for Pointers
// Features: &, *, Pointer Arithmetic, Double Pointers, Integration
// Returns: 0 on Success, Line Number on Failure

int main() {
    int a = 10;
    int b = 20;
    int check = 0;
    
    // ============================================================
    // SECTION 1: BASIC POINTER OPERATIONS
    // ============================================================

    // Test 1.1: Address-of (&) and Dereference (*) - Read
    int* p = &a; // p now holds the address of 'a'
    
    if (*p != 10) {
        return 18; // FAILED: Basic dereference read
    }

    // Test 1.2: Dereference - Write
    // Changing *p should change 'a'
    *p = 50; 
    
    if (a != 50) {
        return 26; // FAILED: Basic dereference write
    }

    // Test 1.3: Pointer Re-assignment
    p = &b; // p now points to 'b'
    *p = 100;
    
    if (b != 100) {
        return 34; // FAILED: Pointer reassignment
    }
    // Ensure 'a' was NOT touched
    if (a != 50) {
        return 38; // FAILED: Reassignment side-effect check
    }

    // ============================================================
    // SECTION 2: POINTER ARITHMETIC (SCALING)
    // ============================================================
    // This is the #1 bug in compilers. 
    // int* p; p+1 should add 4 bytes (sizeof int).
    // char* c; c+1 should add 1 byte (sizeof char).

    int arr[3];
    arr[0] = 100;
    arr[1] = 200;
    arr[2] = 300;
    
    p = &arr[0]; // Point to start

    // Test 2.1: Access via arithmetic expression
    // *(p + 1) should access arr[1]
    if (*(p + 1) != 200) {
        return 58; // FAILED: Int pointer addition (offset check)
    }

    // Test 2.2: Incrementing the pointer variable
    p = p + 1; // Now points to arr[1]
    if (*p != 200) {
        return 64; // FAILED: Int pointer increment
    }
    
    p = p + 1; // Now points to arr[2]
    if (*p != 300) {
        return 69; // FAILED: Int pointer increment 2
    }

    // Test 2.3: Char Pointer Arithmetic
    char carr[3];
    carr[0] = 'A';
    carr[1] = 'B';
    carr[2] = 'C';
    
    char* cp = &carr[0];
    
    // cp + 1 should jump 1 byte, not 4
    if (*(cp + 1) != 'B') {
        return 81; // FAILED: Char pointer arithmetic
    }

    // ============================================================
    // SECTION 3: POINTERS AND LOOPS (ARRAY TRAVERSAL)
    // ============================================================
    // Iterating an array using pointers is more efficient in assembly
    // than array indexing (no multiply instruction needed inside loop).
    
    // Reset array
    arr[0] = 1; arr[1] = 2; arr[2] = 3;
    p = &arr[0];
    int sum = 0;
    int i = 0;
    
    // Loop 3 times
    while (i < 3) {
        sum = sum + *p; // Add current value
        p = p + 1;      // Move to next int
        i = i + 1;
    }
    // Sum = 1 + 2 + 3 = 6
    if (sum != 6) {
        return 103; // FAILED: Loop traversal with pointers
    }

    // ============================================================
    // SECTION 4: DOUBLE POINTERS (int**)
    // ============================================================
    // Handling multiple levels of indirection.

    a = 999;
    p = &a;       // p -> a
    int** pp = &p; // pp -> p -> a
    
    // Test 4.1: Double Dereference Read
    if (**pp != 999) {
        return 116; // FAILED: Double pointer read
    }

    // Test 4.2: Double Dereference Write
    **pp = 777; // Should change 'a' to 777
    if (a != 777) {
        return 122; // FAILED: Double pointer write
    }

    // Test 4.3: Modifying the middle pointer
    // Change 'p' via 'pp' to point to 'b'
    b = 888;
    *pp = &b; // Now p -> b
    
    if (*p != 888) {
        return 130; // FAILED: Modifying ptr via dbl-ptr
    }
    if (**pp != 888) {
        return 133; // FAILED: Verify change
    }

    // ============================================================
    // SECTION 5: INTEGRATION (ARITHMETIC & LOGIC)
    // ============================================================

    a = 10;
    b = 20;
    int* p1 = &a;
    int* p2 = &b;

    // Test 5.1: Arithmetic between two pointers values
    // (*p1 + *p2) = 10 + 20 = 30
    int res = *p1 + *p2;
    if (res != 30) {
        return 148; // FAILED: Arithmetic with dereferenced values
    }

    // Test 5.2: Logic comparison of pointer values
    if (*p1 >= *p2) { // 10 >= 20 is False
        return 153; // FAILED: Logic compare (>)
    }

    // Test 5.3: Pointer in 'switch'
    // Switch on the VALUE pointed to by p1 (10)
    switch (*p1) {
        case 10:
            res = 1;
            break;
        default:
            res = 0;
    }
    if (res != 1) {
        return 166; // FAILED: Switch on dereferenced value
    }

    // Test 5.4: NULL Pointer Logic
    // We can't dereference NULL (crash), but we can compare logic
    int* null_ptr = 0; // or NULL
    
    if (null_ptr != 0) {
        return 174; // FAILED: Null pointer check
    }
    
    // Ensure we don't enter this block
    if (null_ptr) {
        return 179; // FAILED: Null pointer evaluated to true
    }

    return 0; // ALL TESTS PASSED
}
