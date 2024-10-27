// Exhaustive Test Suite for Multi-Level Pointers
// Features: int**, int***, Dynamic 2D Arrays, Pointer Swapping
// Integration: malloc, free, functions, loops, structs
// Returns: 0 on Success, Line Number on Failure

extern int* malloc(int size);
extern void free(int* ptr);

// Structure containing a double pointer
struct Matrix {
    int rows;
    int cols;
    int** data; // Pointer to array of pointers
};

// Helper: Swap where two pointers point to
// Takes int** because we want to modify int*
void swap_ptrs(int** a, int** b) {
    int* temp = *a;
    *a = *b;
    *b = temp;
}

// Helper: Allocate an integer and return via argument
// Uses int** to modify the caller's int*
void alloc_setup(int** p, int val) {
    *p = malloc(4);
    **p = val; // Dereference twice to write value
}

int main() {
    int val = 10;
    int* p = &val;
    int** pp = &p;
    
    // ==========================================
    // 1. Basic Double Indirection
    // ==========================================
    
    // Read: **pp should be 10
    if (**pp != 10) {
        return 42; // FAILED: Double dereference read
    }
    
    // Write: Change val via pp
    **pp = 20;
    if (val != 20) {
        return 47; // FAILED: Double dereference write
    }
    
    // Check intermediate address
    if (*pp != p) {
        return 51; // FAILED: Single dereference of double pointer
    }

    // ==========================================
    // 2. Triple Indirection (int***)
    // ==========================================
    int*** ppp = &pp;
    
    // Write via triple pointer
    ***ppp = 30;
    if (val != 30) {
        return 61; // FAILED: Triple dereference write
    }
    
    // ==========================================
    // 3. Pointer Arithmetic on int**
    // ==========================================
    // We need an array of pointers to test this.
    int v1 = 100;
    int v2 = 200;
    int* ptr_arr[2];
    ptr_arr[0] = &v1;
    ptr_arr[1] = &v2;
    
    int** pp_iter = &ptr_arr[0]; // Points to start of array
    
    // Test 3.1: Base access
    if (**pp_iter != 100) {
        return 77; // FAILED: Pointer to array of pointers
    }
    
    // Test 3.2: Arithmetic (pp_iter + 1)
    // Should jump 4 bytes (size of int*), not size of int
    pp_iter = pp_iter + 1; 
    
    if (**pp_iter != 200) {
        return 85; // FAILED: Arithmetic on double pointer
    }

    // ==========================================
    // 4. Function Arguments (Pass by Reference)
    // ==========================================
    
    int x = 1;
    int y = 2;
    int* px = &x;
    int* py = &y;
    
    // Swap px and py (pointers), not x and y (values)
    swap_ptrs(&px, &py);
    
    // Now px should point to y (2), py to x (1)
    if (*px != 2) {
        return 100; // FAILED: Pointer swap logic (px)
    }
    if (*py != 1) {
        return 103; // FAILED: Pointer swap logic (py)
    }
    
    // ==========================================
    // 5. Dynamic Output Parameter
    // ==========================================
    int* dyn_p = 0;
    // Pass address of pointer
    alloc_setup(&dyn_p, 999); 
    
    if (dyn_p == 0) {
        return 114; // FAILED: Function failed to update pointer
    }
    if (*dyn_p != 999) {
        return 117; // FAILED: Function failed to set value
    }
    free(dyn_p);

    // ==========================================
    // 6. Dynamic 2D Array (The Matrix Test)
    // ==========================================
    struct Matrix m;
    m.rows = 3;
    m.cols = 3;
    
    // 1. Allocate array of ROW pointers (int**)
    m.data = (int**) malloc(m.rows * 4); // 3 * 4 bytes
    
    int r = 0;
    int c = 0;
    
    // 2. Allocate columns for each row
    for (r = 0; r < m.rows; r = r + 1) {
        // Uses pointer arithmetic on m.data to find slot
        // m.data[r] = malloc(...)
        // Compiler must handle: *(m.data + r) = ...
        *(m.data + r) = (int*) malloc(m.cols * 4);
        
        // Fill data
        for (c = 0; c < m.cols; c = c + 1) {
            int* row_ptr = *(m.data + r);
            *(row_ptr + c) = (r * 10) + c;
        }
    }
    
    // 3. Verify Data
    // m.data[1][2] should be 12
    int* row_1 = *(m.data + 1);
    if (*(row_1 + 2) != 12) {
        return 149; // FAILED: 2D Array Access
    }
    
    // 4. Access via chain
    // Check m.data[2][0] = 20
    int** row_start = m.data + 2; // Points to the 3rd pointer
    int* col_start = *row_start;  // The 3rd array
    if (*col_start != 20) {
        return 157; // FAILED: Complex 2D pointer chain
    }

    // Cleanup (Optional in test, but good practice)
    for (r = 0; r < m.rows; r = r + 1) {
        free(*(m.data + r));
    }
    free((int*)m.data); // Cast just to silence warnings if strict

    return 0; // ALL TESTS PASSED
}