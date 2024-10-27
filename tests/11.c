// Exhaustive Test Suite for 'static' keyword
// Features: Global static, Local static, Static Arrays, Static Structs
// Integration: Recursion, Pointers, Loops
// Returns: 0 on Success, Line Number on Failure

// --- 1. Global Static (File Scope) ---
static int g_shared = 100;
static int g_zero; // Should be zero-initialized (BSS)

struct State {
    int val;
    int visited;
};

// --- Helper Functions ---

// Scenario 2: Simple Persistence (Counter)
// If 'count' is on stack, this returns 1 every time.
// If 'count' is static, it returns 1, 2, 3...
int counter() {
    static int count = 0;
    count = count + 1;
    return count;
}

// Scenario 3: Shadowing (Local Static hides Global Static)
int check_shadow() {
    // This 'g_shared' is distinct from the global one (100)
    static int g_shared = 10; 
    g_shared = g_shared + 1;
    return g_shared;
}

// Scenario 4: Pointers to Local Static
// Returning address of a local var is illegal/unsafe.
// Returning address of a static var is VALID because it lives in .data
int* get_static_address() {
    static int persistent_val = 555;
    return &persistent_val;
}

// Scenario 5: Static Array (Memory Persistence)
int array_memory(int index, int val) {
    // Array should retain values between calls
    static int arr[5]; 
    
    if (index >= 0) {
        arr[index] = val; // Write mode
        return 1;
    } else {
        // Read mode (index is negative, look at stored values)
        // Sum first 3 elements
        return arr[0] + arr[1] + arr[2];
    }
}

// Scenario 6: Static Struct in Recursion
// Standard locals are unique per recursion frame.
// Static locals are SHARED across all recursion frames.
int recursive_shared(int n) {
    static struct State s; // .val=0, .visited=0 implicitly
    
    s.visited = s.visited + 1; // Count total calls
    
    if (n <= 0) {
        return s.visited;
    }
    
    return recursive_shared(n - 1);
}

// ==========================================
// Main Function
// ==========================================
int main() {
    int res = 0;
    int i = 0;

    // --- Test 1: Global Static Basics ---
    if (g_shared != 100) {
        return 79; // FAILED: Global static init
    }
    if (g_zero != 0) {
        return 82; // FAILED: Global static zero init
    }
    g_shared = 200; // Modify global

    // --- Test 2: Local Static Persistence ---
    // Call 1 -> returns 1
    if (counter() != 1) {
        return 89; // FAILED: Static persistence (Call 1)
    }
    // Call 2 -> returns 2
    if (counter() != 2) {
        return 93; // FAILED: Static persistence (Call 2)
    }
    // Call 3 -> returns 3
    if (counter() != 3) {
        return 97; // FAILED: Static persistence (Call 3)
    }

    // --- Test 3: Shadowing ---
    // Global g_shared is 200.
    // Local static g_shared starts at 10.
    res = check_shadow(); // Returns 11
    
    if (res != 11) {
        return 106; // FAILED: Local static shadowing logic
    }
    // Verify Global was NOT touched
    if (g_shared != 200) {
        return 110; // FAILED: Local static overwrote global variable
    }
    // Verify Local persisted
    if (check_shadow() != 12) {
        return 114; // FAILED: Shadowed variable persistence
    }

    // --- Test 4: Pointer to Static (Lifetime check) ---
    int* ptr = get_static_address();
    
    if (*ptr != 555) {
        return 121; // FAILED: Read static via pointer
    }
    
    // Modify via pointer
    *ptr = 777;
    
    // Call function again to get address (should be same address, same value)
    int* ptr2 = get_static_address();
    
    if (*ptr2 != 777) {
        return 131; // FAILED: Modification via pointer didn't persist
    }

    // --- Test 5: Static Arrays ---
    // If 'arr' was on stack, it would be reset or contain garbage on every call.
    array_memory(0, 10);
    array_memory(1, 20);
    array_memory(2, 30);
    
    // Do some stack heavy operations to try and corrupt the stack
    counter(); counter();
    
    // Read back sum: 10 + 20 + 30 = 60
    res = array_memory(-1, 0);
    
    if (res != 60) {
        return 147; // FAILED: Static array persistence
    }

    // --- Test 6: Recursion with Static Struct ---
    // recursive_shared(3) calls:
    // n=3 (visited=1) -> recurse
    // n=2 (visited=2) -> recurse
    // n=1 (visited=3) -> recurse
    // n=0 (visited=4) -> return 4
    res = recursive_shared(3);
    
    if (res != 4) {
        return 160; // FAILED: Static struct shared across recursion
    }
    
    // Call again from root. 'visited' remembers 4.
    // n=1 (visited=5) -> recurse
    // n=0 (visited=6) -> return 6
    res = recursive_shared(1);
    
    if (res != 6) {
        return 169; // FAILED: Static struct persistence across root calls
    }

    // --- Test 7: Static inside Loops ---
    // Logic: 'val' inits to 5 ONCE.
    // i=0: val becomes 6
    // i=1: val becomes 7
    // i=2: val becomes 8
    int loop_res = 0;
    for (i = 0; i < 3; i = i + 1) {
        static int val = 5;
        val = val + 1;
        loop_res = val;
    }
    if (loop_res != 8) {
        return 183; // FAILED: Static inside for-loop block
    }

    return 0; // ALL TESTS PASSED
}