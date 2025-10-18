// Error Test 1: Typedef with storage class should fail
typedef static enum {
    A, B, C
} BadEnum;

int main() {
    return 0;
}
