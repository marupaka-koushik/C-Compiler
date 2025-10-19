// Error test: continue in switch (should only be in loops)
// Actually, this is valid in C - continue just applies to the loop
// Let me change this to a true error case
int main() {
    int x = 2;
    if (x > 0) {
        continue;  // ERROR: continue not in a loop
    }
    return 0;
}
