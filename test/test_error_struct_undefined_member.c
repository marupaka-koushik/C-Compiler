// Error: Accessing undefined structure member

struct Point {
    int x;
    int y;
};

int main() {
    struct Point p;
    p.x = 10;
    p.y = 20;
    
    // ERROR: 'z' is not a member of struct Point
    int val = p.z;
    
    return 0;
}
