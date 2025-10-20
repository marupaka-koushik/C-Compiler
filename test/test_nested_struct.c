// Test nested struct members

struct Date {
    int day;
    int month;
    int year;
};

struct Employee {
    int id;
    struct Date joinDate;
};

int main() {
    struct Employee emp;
    emp.id = 101;
    emp.joinDate.day = 15;
    emp.joinDate.month = 6;
    emp.joinDate.year = 2023;
    return 0;
}
