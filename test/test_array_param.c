// Test array parameter without reference return

void printFirst(int arr[], int size) {
    int x = arr[0];
}

int main() {
    int arr[5] = {1, 2, 3, 4, 5};
    printFirst(arr, 5);
    return 0;
}
