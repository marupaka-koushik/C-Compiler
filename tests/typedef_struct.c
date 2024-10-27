typedef int Integer;

struct Coord {
    Integer x;
    Integer y;
};

typedef struct Coord Point;

int main() {
    Point p;
    p.x = 1;
    p.y = 2;
    
    if (p.y != 2) {
        return 1;
    }
    
    return 0;
}
