// ERROR: Address-of on non-lvalue (literal)
int main() {
    int *ptr = &10;  // ERROR: Cannot take address of literal
    return 0;
}
