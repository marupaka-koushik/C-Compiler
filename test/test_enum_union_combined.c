// Combined enum and union test

enum Status {
    IDLE,
    RUNNING,
    STOPPED
};

union Data {
    int i;
    double d;
    char c;
};

int main() {
    // Test enum
    enum Status s = RUNNING;
    int statusCode = IDLE;
    
    if (s == RUNNING) {
        statusCode = 1;
    }
    
    // Test union
    union Data data;
    data.i = 42;
    data.d = 3.14;
    data.c = 'A';
    
    // Combine both
    enum Status finalStatus = STOPPED;
    
    return statusCode;
}
