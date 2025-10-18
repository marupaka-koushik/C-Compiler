typedef enum {
    RED,
    GREEN,
    BLUE,
    YELLOW
} Color;

int main() {
    Color c1 = RED;
    Color c2 = GREEN;
    Color c3 = BLUE;
    
    if (c1 == RED) {
        c2 = YELLOW;
    }
    
    int sum = c1 + c2 + c3;
    return sum;
}
