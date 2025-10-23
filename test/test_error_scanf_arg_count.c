// Error: scanf format-argument count mismatch

int main() {
    int x, y;
    
    // ERROR: Three format specifiers (%d %d %d) but only two arguments (&x, &y)
    scanf("%d %d %d", &x, &y);
    
    return 0;
}
