#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_COURSE 50

typedef struct {
    int id;
    char name[MAX_NAME];
    int age;
    char course[MAX_COURSE];
    int numSubjects;
    float *grades;
    float gpa;
} Student;

Student *students = NULL;
int count = 0;


float calculateGPA(float grades[], int n) {
    float sum = 0;
    for (int i = 0; i < n; i++) {
        sum += grades[i];
    }
    return sum / n;
}

void clearInputBuffer() {
    while (getchar() != '\n');
}


void addStudent() {
    Student s;

    printf("Enter ID: ");
    scanf("%d", &s.id);

    // Check duplicate ID
    for (int i = 0; i < count; i++) {
        if (students[i].id == s.id) {
            printf("ID already exists!\n");
            return;
        }
    }

    printf("Enter Name: ");
    scanf(" %[^\n]", s.name);

    printf("Enter Age: ");
    scanf("%d", &s.age);

    printf("Enter Course: ");
    scanf(" %[^\n]", s.course);

    printf("Number of subjects: ");
    scanf("%d", &s.numSubjects);

    if (s.numSubjects <= 0) {
        printf("Invalid number!\n");
        return;
    }

    s.grades = (float *)malloc(s.numSubjects * sizeof(float));

    for (int i = 0; i < s.numSubjects; i++) {
        printf("Grade %d: ", i + 1);
        scanf("%f", &s.grades[i]);

        if (s.grades[i] < 0 || s.grades[i] > 100) {
            printf("Invalid grade!\n");
            free(s.grades);
            return;
        }
    }

    s.gpa = calculateGPA(s.grades, s.numSubjects);

    students = (Student *)realloc(students, (count + 1) * sizeof(Student));
    students[count] = s;
    count++;

    printf("Student added successfully!\n");
}


void displayStudents() {
    if (count == 0) {
        printf("No records.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("\nID: %d\nName: %s\nAge: %d\nCourse: %s\nGPA: %.2f\n",
               students[i].id, students[i].name,
               students[i].age, students[i].course,
               students[i].gpa);
    }
}


void searchByID() {
    int id;
    printf("Enter ID: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            printf("Found: %s (GPA %.2f)\n", students[i].name, students[i].gpa);
            return;
        }
    }
    printf("Not found.\n");
}

void searchByName() {
    char name[MAX_NAME];
    printf("Enter Name: ");
    scanf(" %[^\n]", name);

    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].name, name) == 0) {
            printf("Found: ID %d, GPA %.2f\n", students[i].id, students[i].gpa);
            return;
        }
    }
    printf("Not found.\n");
}


void deleteStudent() {
    int id;
    printf("Enter ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {

            free(students[i].grades);

            for (int j = i; j < count - 1; j++) {
                students[j] = students[j + 1];
            }

            count--;
            students = realloc(students, count * sizeof(Student));

            printf("Deleted successfully!\n");
            return;
        }
    }
    printf("Student not found.\n");
}


void sortByGPA() {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (students[j].gpa < students[j + 1].gpa) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
    printf("Sorted by GPA\n");
}

void sortByName() {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(students[j].name, students[j + 1].name) > 0) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
    printf("Sorted by Name\n");
}


void saveToFile() {
    FILE *fp = fopen("records.dat", "wb");

    if (!fp) {
        printf("File error!\n");
        return;
    }

    fwrite(&count, sizeof(int), 1, fp);

    for (int i = 0; i < count; i++) {
        fwrite(&students[i], sizeof(Student), 1, fp);
        fwrite(students[i].grades, sizeof(float), students[i].numSubjects, fp);
    }

    fclose(fp);
    printf("Saved to file\n");
}

void loadFromFile() {
    FILE *fp = fopen("records.dat", "rb");

    if (!fp) {
        printf("No saved data.\n");
        return;
    }

    fread(&count, sizeof(int), 1, fp);
    students = malloc(count * sizeof(Student));

    for (int i = 0; i < count; i++) {
        fread(&students[i], sizeof(Student), 1, fp);

        students[i].grades = malloc(students[i].numSubjects * sizeof(float));
        fread(students[i].grades, sizeof(float), students[i].numSubjects, fp);

        students[i].gpa = calculateGPA(students[i].grades, students[i].numSubjects);
    }

    fclose(fp);
    printf("Data loaded\n");
}

void report() {
    if (count == 0) {
        printf("No data.\n");
        return;
    }

    float sum = 0, max = students[0].gpa, min = students[0].gpa;

    for (int i = 0; i < count; i++) {
        sum += students[i].gpa;
        if (students[i].gpa > max) max = students[i].gpa;
        if (students[i].gpa < min) min = students[i].gpa;
    }

    printf("\nAverage GPA: %.2f\n", sum / count);
    printf("Highest GPA: %.2f\n", max);
    printf("Lowest GPA: %.2f\n", min);
}

void menu() {
    printf("\n===== MENU =====\n");
    printf("1. Add Student\n");
    printf("2. Display\n");
    printf("3. Search by ID\n");
    printf("4. Search by Name\n");
    printf("5. Delete\n");
    printf("6. Sort by GPA\n");
    printf("7. Sort by Name\n");
    printf("8. Save\n");
    printf("9. Load\n");
    printf("10. Report\n");
    printf("0. Exit\n");
}

int main() {
    int choice;

    do {
        menu();
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchByID(); break;
            case 4: searchByName(); break;
            case 5: deleteStudent(); break;
            case 6: sortByGPA(); break;
            case 7: sortByName(); break;
            case 8: saveToFile(); break;
            case 9: loadFromFile(); break;
            case 10: report(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }

    } while (choice != 0);

    for (int i = 0; i < count; i++) {
        free(students[i].grades);
    }
    free(students);

    return 0;
}