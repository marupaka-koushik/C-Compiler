int main() {
    int x = 10;
    int *ptr = &x;
    int **pptr = &ptr;
    int w = **pptr;
    return 0;
}
