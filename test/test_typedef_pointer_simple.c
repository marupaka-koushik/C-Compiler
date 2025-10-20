typedef int* IntPtr;

int main() {
    int value = 42;
    IntPtr p = &value;
    return *p;
}
