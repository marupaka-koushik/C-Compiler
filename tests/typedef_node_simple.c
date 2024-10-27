typedef int Integer;

typedef struct Node {
    Integer val;
    struct Node* next;
} Node;

typedef Node* NodePtr;

int main() {
    NodePtr head;
    head->val = 10;
    return 0;
}
