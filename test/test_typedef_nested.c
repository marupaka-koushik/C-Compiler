// Test 5: Nested typedef
typedef int Integer;
typedef Integer* IntegerPtr;
typedef IntegerPtr* IntegerPtrPtr;

int main() {
    Integer x = 100;
    IntegerPtr p = &x;
    IntegerPtrPtr pp = &p;
    
    Integer val1 = *p;
    Integer val2 = **pp;
    
    return 0;
}
