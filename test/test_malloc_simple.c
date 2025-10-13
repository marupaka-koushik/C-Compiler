// Simple malloc test

int main() {
    int *ptr = malloc(10);
    *ptr = 42;
    free(ptr);
    return 0;
}
