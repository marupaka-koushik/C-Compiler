// Simple pointer to structure test

struct Point {
    int x;
    int y;
};

int main() {
    struct Point p;
    p.x = 10;
    
    struct Point *ptr;
    ptr = &p;
    ptr->x = 20;
    
    return 0;
}
