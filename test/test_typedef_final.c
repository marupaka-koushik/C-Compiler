// Final typedef demonstration showing symbol table linking

// Basic typedefs
typedef int MyInt;
typedef float MyFloat;
typedef double MyDouble;

// Pointer typedefs
typedef int* IntPointer;
typedef char* String;
typedef void* VoidPtr;

// Array typedefs
typedef int IntArray[10];
typedef float Matrix[3][3];

// Struct with typedef
typedef struct Employee {
    int id;
    float salary;
    char name[50];
} Employee;

// Anonymous struct typedef
typedef struct {
    int hour;
    int minute;
    int second;
} Time;

// Regular struct without typedef
struct Department {
    int deptId;
    char deptName[100];
};

int main() {
    // Regular variable declarations
    int regularInt;
    float regularFloat;
    int* regularPtr;
    int regularArray[5];
    
    // Struct declarations
    struct Employee emp;
    struct Department dept;
    
    // These would work if we had full typedef support in grammar
    // MyInt x;           // Would need lexer feedback
    // String str;        // Would need lexer feedback
    // Employee e;        // Would need lexer feedback
    
    return 0;
}
