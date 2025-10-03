// Test reference return with arrays

int& getElement(int arr[], int index) {
    return arr[index];
}

int main() {
    int arr[5] = {1, 2, 3, 4, 5};
    int &elem = getElement(arr, 2);
    elem = 99;
    return 0;
}
