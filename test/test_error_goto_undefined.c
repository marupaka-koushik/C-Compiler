// Error: Goto to undefined label

int main() {
    int x = 10;
    
    // ERROR: Label 'undefined_label' does not exist
    goto undefined_label;
    
    x = 20;
    
    return 0;
}
