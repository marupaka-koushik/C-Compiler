// Comprehensive typedef test
typedef int Integer;
typedef float Real;
typedef int* IntPtr;
typedef int IntArray[10];

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct {
    float x;
    float y;
    float z;
} Vector3D;

int main() {
    // Regular declarations work fine
    int num;
    float val;
    int* ptr;
    int arr[5];
    
    // Struct declarations also work
    struct Point p;
    
    return 0;
}
