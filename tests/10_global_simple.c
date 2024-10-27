int g_val = 0;

void set_global() {
    g_val = 42;
}

int main() {
    set_global();
    if (g_val != 42) {
        return 1;
    }
    return 0;
}
