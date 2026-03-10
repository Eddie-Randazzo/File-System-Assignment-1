#include <iostream>
#include <cstdio>

struct Student {
    int id;
    char name[30];
    int age;
};
int main() {
    FILE* file = fopen("students.db", "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    
    Student s;
    int recordNumber = 3;

    long offset = (recordNumber - 1) * sizeof(Student);

    if (fseek(file, offset, SEEK_SET) != 0) {
        perror("Error seeking");
        fclose(file);
        return 1;
    }

    if (fread(&s, sizeof(Student), 1, file) == 1) {
        std::cout << "--- Record #" << recordNumber << " ---" << std:endl;
        std::cout << "ID: " << s.id << std::endl;
        std::cout << "Name: " << s.name << std::endl;
        std::cout << "Age: " << s.age << std::endl;
    }

    fclose(file);
    return 0;
}