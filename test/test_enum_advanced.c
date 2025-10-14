// Advanced enum test combining with other features

enum ErrorCode {
    SUCCESS = 0,
    ERROR_INVALID = -1,
    ERROR_NOMEM = -2,
    ERROR_IO = -3
};

enum Priority {
    LOW = 1,
    MEDIUM,
    HIGH
};

struct Task {
    int id;
    enum Priority priority;
    enum ErrorCode status;
};

int main(int argc, char **argv) {
    // Enum with struct
    struct Task t;
    t.id = 1;
    t.priority = HIGH;
    t.status = SUCCESS;
    
    // Enum with switch
    enum Priority p = MEDIUM;
    int value;
    switch (p) {
        case LOW:
            value = 10;
            break;
        case MEDIUM:
            value = 20;
            break;
        case HIGH:
            value = 30;
            break;
    }
    
    // Enum with malloc and sizeof
    int size = sizeof(enum Priority);
    enum ErrorCode *errors = (enum ErrorCode*)malloc(size * 3);
    errors[0] = SUCCESS;
    errors[1] = ERROR_INVALID;
    errors[2] = ERROR_NOMEM;
    
    // Enum with recursion
    enum ErrorCode result = SUCCESS;
    if (argc > 1) {
        result = ERROR_INVALID;
    }
    
    // Enum return
    return result;
}
