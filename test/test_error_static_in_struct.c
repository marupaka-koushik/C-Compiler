// Error: Static members in struct (not allowed in C)

struct TestStruct {
    static int member;  // Error: C doesn't support static struct members
    int normal_member;
};

int main() {
    struct TestStruct s;
    s.normal_member = 10;
    return 0;
}
