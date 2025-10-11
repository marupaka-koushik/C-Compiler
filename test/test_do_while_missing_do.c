// Error Test 2: Missing do keyword
int main() {
    int x = 0;
    {
        x = x + 1;
    } while (x < 5);  // Should error: while without do
    return x;
}
