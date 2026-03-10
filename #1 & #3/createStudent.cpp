#include <iostream>
#include <cstdio>

struct Student {
    int id;
    char name[30];
    int age;
};

int main() {
    FILE* file = fopen("students.db", "wb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    Student students[5] = {
        {101, "Alice", 20},
        {102, "Bob", 21},
        {103, "Charlie", 22},
        {104, "David", 23},
        {105, "Eve", 19}
    };

    fwrite(students, sizeof(Student), 5, file);
    std::cout << "Binary file 'students.db' created successfully." << std::endl;
    return 0;
}