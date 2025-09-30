// Combining until loop with other compiler features

enum Status {
    PENDING,
    ACTIVE,
    DONE
};

struct Task {
    int id;
    enum Status status;
};

int main() {
    // until with enum
    enum Status s = PENDING;
    int counter = 0;
    until (s == DONE) {
        counter = counter + 1;
        if (counter >= 3) {
            s = DONE;
        }
    }
    
    // until with struct
    struct Task task;
    task.id = 0;
    task.status = PENDING;
    
    until (task.status == DONE) {
        task.id = task.id + 1;
        if (task.id >= 5) {
            task.status = DONE;
        }
    }
    
    // until with pointers
    int arr[5];
    int *ptr = arr;
    int idx = 0;
    
    until (idx >= 5) {
        ptr[idx] = idx * 2;
        idx = idx + 1;
    }
    
    return 0;
}
