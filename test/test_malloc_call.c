// Test malloc call without pointer assignment

int main() {
    malloc(10);
    free(0);
    return 0;
}
