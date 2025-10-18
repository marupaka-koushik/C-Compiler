// Test struct with multilevel pointers

struct Node {
    int data;
    struct Node *next;
    struct Node **prev_ptr;
};

typedef struct Node* NodePtr;
typedef NodePtr* NodePtrPtr;

int main() {
    // Basic struct with pointers
    struct Node n1, n2, n3;
    n1.data = 10;
    n2.data = 20;
    n3.data = 30;
    
    // Create pointer chain
    n1.next = &n2;
    n2.next = &n3;
    n3.next = &n1;
    
    // Pointer to struct
    struct Node *p = &n1;
    int val1 = p->data;
    
    // Double pointer to struct
    struct Node **pp = &p;
    int val2 = (*pp)->data;
    
    // Triple pointer to struct
    struct Node ***ppp = &pp;
    int val3 = (**ppp)->data;
    
    // Using typedef
    NodePtr np = &n1;
    int val4 = np->data;
    
    NodePtrPtr npp = &np;
    int val5 = (*npp)->data;
    
    // Array of struct pointers
    struct Node *arr[3];
    arr[0] = &n1;
    arr[1] = &n2;
    arr[2] = &n3;
    
    struct Node **ptr_to_arr = arr;
    int val6 = (*ptr_to_arr)->data;
    
    return val1 + val2 + val3 + val4 + val5 + val6;
}
