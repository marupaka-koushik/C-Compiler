// Error: Accessing argv out of bounds

int main(int argc, char *argv[]) {
    // Error: accessing argv beyond argc
    printf("Argument: %s\n", argv[argc]);  // argv[argc] is always NULL
    printf("Invalid: %s\n", argv[argc + 1]);  // Out of bounds
    
    return 0;
}
