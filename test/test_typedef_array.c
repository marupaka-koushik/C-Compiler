// Test 4: Typedef with arrays
typedef int IntArray[10];

int main() {
    IntArray arr;
    arr[0] = 10;
    arr[1] = 20;
    int val = arr[0];
    
    return 0;
}
