// Exhaustive Test Suite for Multi-Dimensional Arrays
// Features: 2D/3D Arrays, Static Multi-dim, Struct Multi-dim
// Integration: Loops, Pointers, Functions
// Returns: 0 on Success, Line Number on Failure

// --- 1. Struct with 2D Array ---
struct Grid {
    int id;
    int matrix[4][4]; // 16 integers
};

// --- 2. Global Static 2D Array ---
static int g_table[2][3]; // 6 integers

// --- Helper: Pass 2D array to function ---
// Note: In C, passing int arr[2][2] decays to int (*arr)[2].
// If your compiler is strict, you might need to pass int* and calculate manually.
// This test assumes standard array syntax support.
int sum_2d(int* flat_ptr, int rows, int cols) {
    int i = 0;
    int sum = 0;
    int total_cells = rows * cols;
    
    // Iterate as flat array to verify memory layout
    for (i = 0; i < total_cells; i = i + 1) {
        sum = sum + flat_ptr[i];
    }
    return sum;
}

int main() {
    int i = 0;
    int j = 0;
    int k = 0;
    int val = 0;

    // ==========================================
    // SECTION 1: BASIC 2D ARRAYS
    // ==========================================
    // int arr[2][3]; -> 2 Rows, 3 Cols. Total 6 ints.
    int arr[2][3];
    
    // Row 0
    arr[0][0] = 10;
    arr[0][1] = 20;
    arr[0][2] = 30;
    
    // Row 1
    arr[1][0] = 40;
    arr[1][1] = 50;
    arr[1][2] = 60;
    
    // Test 1.1: Direct Access
    if (arr[0][1] != 20) {
        return 53; // FAILED: 2D Array Read (Row 0)
    }
    if (arr[1][2] != 60) {
        return 56; // FAILED: 2D Array Read (Row 1)
    }

    // Test 1.2: Memory Layout (Contiguous Check)
    // In row-major, arr[0][3] should technically be arr[1][0] in memory.
    // We access via a flat pointer to verify.
    int* p = &arr[0][0];
    
    // Index 3 in flat memory should be 40 (arr[1][0])
    if (*(p + 3) != 40) {
        return 65; // FAILED: Row-Major layout check
    }

    // ==========================================
    // SECTION 2: NESTED LOOPS & LOGIC
    // ==========================================
    int grid[5][5];
    int sum = 0;
    
    // Fill Logic: grid[i][j] = i + j
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            grid[i][j] = i + j;
        }
    }
    
    // Verify grid[4][4] = 4+4 = 8
    if (grid[4][4] != 8) {
        return 81; // FAILED: Nested loop write to 2D array
    }
    
    // Sum using nested loops
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            // Logic: Sum only diagonal elements
            if (i == j) {
                sum = sum + grid[i][j];
            }
        }
    }
    // Diagonal: 0+0, 1+1, 2+2, 3+3, 4+4 -> 0+2+4+6+8 = 20
    if (sum != 20) {
        return 95; // FAILED: Logic/If inside nested 2D iteration
    }

    // ==========================================
    // SECTION 3: 3D ARRAYS
    // ==========================================
    // int cube[2][2][2]
    // Address: Base + ((i*2*2) + (j*2) + k) * 4
    int cube[2][2][2];
    
    cube[0][0][0] = 1;
    cube[1][1][1] = 999;
    cube[0][1][0] = 5; // "Middle"
    
    if (cube[1][1][1] != 999) {
        return 108; // FAILED: 3D Array Access
    }
    
    // Verify no overlap
    if (cube[0][0][0] != 1) {
        return 112; // FAILED: 3D Array Overlap/Corruption
    }

    // ==========================================
    // SECTION 4: STRUCTS WITH 2D ARRAYS
    // ==========================================
    struct Grid g;
    g.id = 77;
    
    // Fill struct matrix
    g.matrix[2][2] = 100;
    g.matrix[3][3] = 200;
    
    if (g.matrix[2][2] != 100) {
        return 124; // FAILED: 2D Array inside Struct
    }
    
    // Offset check: 
    // g.id (0)
    // g.matrix (4)
    // g.matrix[3][3] index = 15. Offset = 4 + (15 * 4) = 64.
    // We can check this via pointer math if we really wanted to, 
    // but relying on values is safer for the test.
    if (g.matrix[3][3] != 200) {
        return 133; // FAILED: Struct 2D array offset calc
    }

    // ==========================================
    // SECTION 5: STATIC 2D ARRAYS
    // ==========================================
    // Should live in .data section
    
    g_table[0][0] = 10;
    g_table[1][2] = 50;
    
    if (g_table[1][2] != 50) {
        return 144; // FAILED: Global Static 2D array
    }
    
    // Local static 2D
    // Logic: Run loop twice. Array should remember values.
    int static_res = 0;
    for (i = 0; i < 2; i = i + 1) {
        static int local_mat[2][2];
        
        if (i == 0) {
            local_mat[0][0] = 5;
        } else {
            // i == 1, should read 5 from previous iteration
            static_res = local_mat[0][0];
        }
    }
    
    if (static_res != 5) {
        return 159; // FAILED: Local static 2D array persistence
    }

    // ==========================================
    // SECTION 6: FUNCTION PASSING (Decay to Flat)
    // ==========================================
    
    // Fill arr again
    arr[0][0] = 1; arr[0][1] = 1; arr[0][2] = 1;
    arr[1][0] = 1; arr[1][1] = 1; arr[1][2] = 1;
    
    // Pass address of first element
    // Total elements = 6. Sum should be 6.
    val = sum_2d(&arr[0][0], 2, 3);
    
    if (val != 6) {
        return 174; // FAILED: Passing 2D array as pointer to function
    }

    return 0; // ALL TESTS PASSED
}