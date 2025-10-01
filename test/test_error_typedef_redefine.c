// Error: Redefining typedef with different type

typedef int MyInt;
typedef char MyInt;  // Error: MyInt already defined as int

int main() {
    MyInt x = 10;
    return 0;
}
