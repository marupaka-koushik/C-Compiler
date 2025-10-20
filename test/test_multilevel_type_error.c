// Error cases for multilevel pointers

int main() {
    int x = 10;
    int *p = &x;
    
    // Error 1: Type mismatch - assigning single pointer to double pointer
    int **pp;
    pp = p;  // Should error: type mismatch
    
    return 0;
}
