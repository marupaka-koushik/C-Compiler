// Error detection for multidimensional arrays

int main() {
    int matrix[3][3];
    
    // Error: Out of bounds access (should be caught if bounds checking exists)
    int val = matrix[5][5];  // May or may not error depending on implementation
    
    return val;
}
