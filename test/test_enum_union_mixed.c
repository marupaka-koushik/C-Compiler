// Test: Mixing enum and union in same scope
enum Status {
    OK,
    FAIL
};

union Status {  // Different construct, same name - should this be allowed?
    int code;
    char msg;
};

int main() {
    return 0;
}
