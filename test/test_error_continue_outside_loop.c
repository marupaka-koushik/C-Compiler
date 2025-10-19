// Error test: continue statement outside loop
int main() {
    int x = 5;
    continue;  // ERROR: continue must be inside loop
    return 0;
}
