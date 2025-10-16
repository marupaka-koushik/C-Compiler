// Multidimensional arrays with typedef

typedef int Matrix3x3[3][3];
typedef int Row[5];
typedef int Matrix[4][5];
typedef int Cube[2][2][2];

int main() {
    // Using typedef for 2D array
    Matrix3x3 m1;
    m1[0][0] = 1;
    m1[1][1] = 2;
    m1[2][2] = 3;
    
    int val1 = m1[0][0] + m1[1][1] + m1[2][2];
    
    // Using typedef for matrix
    Matrix m2;
    m2[0][0] = 10;
    m2[3][4] = 99;
    
    int val2 = m2[0][0] + m2[3][4];
    
    // Using typedef for 3D array
    Cube c;
    c[0][0][0] = 5;
    c[1][1][1] = 15;
    
    int val3 = c[0][0][0] + c[1][1][1];
    
    // Multiple typedef arrays
    Matrix3x3 m3, m4;
    m3[0][0] = 100;
    m4[2][2] = 200;
    
    int val4 = m3[0][0] + m4[2][2];
    
    return val1 + val2 + val3 + val4;
}
