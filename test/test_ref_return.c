// Test reference return types
int& getMax(int &a, int &b) {
    if (a > b) {
        return a;
    }
    return b;
}

int arr[5];

int& getElement(int index) {
    return arr[index];
}

int main() {
    int x = 10;
    int y = 20;
    int &max = getMax(x, y);
    max = 30;
    
    int &elem = getElement(2);
    elem = 99;
    
    return 0;
}
