// Simple static test to observe behavior

static int staticGlobal = 100;
int normalGlobal = 200;

static int staticFunc(int x) {
    return x + 5;
}

int normalFunc(int x) {
    return x + 10;
}

int main() {
    // Test global access
    int a = staticGlobal;
    int b = normalGlobal;
    
    // Test function calls
    int c = staticFunc(5);
    int d = normalFunc(5);
    
    // Test static local
    static int localStatic = 42;
    int e = localStatic;
    localStatic = 50;
    
    // Test normal local
    int normalLocal = 42;
    int f = normalLocal;
    normalLocal = 50;
    
    return 0;
}
