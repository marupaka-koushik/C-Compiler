// Error: Wrong level of indirection

int main() {
    int x = 10;
    int *p = &x;
    int **pp = &p;
    
    // Error: Cannot assign double pointer to single pointer variable
    int *wrong;
    wrong = pp;  // Type mismatch
    
    return 0;
}
