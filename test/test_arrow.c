struct Point {
    int x;
    int y;
};

int main() {
    struct Point p;
    struct Point *ptr;
    
    p.x = 10;
    p.y = 20;
    ptr = &p;
    
    int a = ptr->x;
    int b = ptr->y;
    
    return 0;
}
