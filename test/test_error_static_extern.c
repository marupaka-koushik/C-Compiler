// Error: Variable declared both static and extern (conflicting storage classes)

static extern int conflicting_var;  // Error: can't be both static and extern

int main() {
    conflicting_var = 10;
    return 0;
}
