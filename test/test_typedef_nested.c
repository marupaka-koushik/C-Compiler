// Edge case testing for typedef
typedef int Integer;
typedef Integer MyInt;

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef Node* NodePtr;

int main() {
    MyInt x;
    Node n;
    NodePtr head;
    
    x = 42;
    n.data = 100;
    head = 0;
    
    return 0;
}
