// Error detection for multilevel pointers

int main() {
    int x = 10;
    int *p = &x;
    int **pp = &p;
    
    // Error: Too many dereferences (should fail)
    int bad = ***pp;  // pp is double pointer, can only dereference twice
    
    return 0;
}
