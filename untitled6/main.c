#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STUDENTS 100

struct Student {
    char id[6];
    char name[31];
    char class[11];
    float math_score, physics_score, chemistry_score;
    float average_score;
};

struct Student students[MAX_STUDENTS];
int numStudents = 0;
void addStudent();
void editStudent();
void deleteStudent();
void displayStudents();
void sortStudents(int order);
void searchByClass();
void searchByScoreRange();

int main() {
    int choice;

    while (1) {
        printf("\n*************** Menu quan ly sinh vien *****************\n");
        printf("1. Add Student\n");
        printf("2. Edit Student\n");
        printf("3. Delete Student\n");
        printf("4. Display Students\n");
        printf("5. Sort Students\n");
        printf("6. Search by Class\n");
        printf("7. Search by Score Range\n");
        printf("8. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                editStudent();
                break;
            case 3:
                deleteStudent();
                break;
            case 4:
                displayStudents();
                break;
            case 5:
                printf("Sort Students:\n1. Ascending\n2. Descending\nEnter choice: ");
                int order;
                scanf("%d", &order);
                sortStudents(order);
                break;
            case 6:
                searchByClass();
                break;
            case 7:
                searchByScoreRange();
                break;
            case 8:
                printf("Exiting program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}


void capitalizeFirstLetter(char *str) {
    int len = strlen(str);
    int capitalizeNext = 1;

    for (int i = 0; i < len; i++) {
        if (isspace(str[i])) {
            capitalizeNext = 1;
        } else if (capitalizeNext) {
            str[i] = toupper(str[i]);
            capitalizeNext = 0;
        } else {
            str[i] = tolower(str[i]);
        }
    }
}

void addStudentToFile(struct Student newStudent) {
    FILE *file = fopen("students.txt", "a");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "%s,%s,%s,%.2f,%.2f,%.2f,%.2f\n",
            newStudent.id,
            newStudent.name,
            newStudent.class,
            newStudent.math_score,
            newStudent.physics_score,
            newStudent.chemistry_score,
            newStudent.average_score);

    fclose(file);
}

void addStudent() {
    struct Student newStudent;

    printf("Enter student ID (5 characters): ");
    scanf("%5s", newStudent.id);

    printf("Enter student name (5-30 characters): ");
    scanf(" %[^\n]", newStudent.name);
    capitalizeFirstLetter(newStudent.name);

    printf("Enter student class (up to 10 characters): ");
    scanf("%10s", newStudent.class);

    do {
        printf("Enter math score (0-10): ");
        scanf("%f", &newStudent.math_score);
    } while (newStudent.math_score < 0 || newStudent.math_score > 10);

    do {
        printf("Enter physics score (0-10): ");
        scanf("%f", &newStudent.physics_score);
    } while (newStudent.physics_score < 0 || newStudent.physics_score > 10);

    do {
        printf("Enter chemistry score (0-10): ");
        scanf("%f", &newStudent.chemistry_score);
    } while (newStudent.chemistry_score < 0 || newStudent.chemistry_score > 10);

    newStudent.average_score = (newStudent.math_score + newStudent.physics_score + newStudent.chemistry_score) / 3.0;
    addStudentToFile(newStudent);

    printf("Student added successfully.\n");
}
void editStudentToFile(struct Student updatedStudent) {
    FILE *file = fopen("students.txt", "w");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < numStudents; i++) {
        fprintf(file, "%s,%s,%s,%.2f,%.2f,%.2f,%.2f\n",
                students[i].id,
                students[i].name,
                students[i].class,
                students[i].math_score,
                students[i].physics_score,
                students[i].chemistry_score,
                students[i].average_score);

        if (strcmp(students[i].id, updatedStudent.id) == 0) {
            // Update the data for the edited student
            fprintf(file, "%s,%s,%s,%.2f,%.2f,%.2f,%.2f\n",
                    updatedStudent.id,
                    updatedStudent.name,
                    updatedStudent.class,
                    updatedStudent.math_score,
                    updatedStudent.physics_score,
                    updatedStudent.chemistry_score,
                    updatedStudent.average_score);
        }
    }

    fclose(file);
}

void editStudent() {
    char targetID[6];
    int found = 0;

    printf("Enter the ID of the student to edit: ");
    scanf("%5s", targetID);

    for (int i = 0; i < numStudents; i++) {
        if (strcmp(students[i].id, targetID) == 0) {
            found = 1;

            printf("Editing student with ID %s:\n", targetID);
            editStudentToFile(students[i]);

            printf("Student information updated successfully.\n");
            break;
        }
    }

    if (!found) {
        printf("Student with ID %s not found.\n", targetID);
    }
}
void deleteStudentFromFile(const char* targetID) {
    FILE* file = fopen("students.txt", "w");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < numStudents; i++) {
        if (strcmp(students[i].id, targetID) != 0) {
            fprintf(file, "%s,%s,%s,%.2f,%.2f,%.2f,%.2f\n",
                    students[i].id,
                    students[i].name,
                    students[i].class,
                    students[i].math_score,
                    students[i].physics_score,
                    students[i].chemistry_score,
                    students[i].average_score);
        }
    }

    fclose(file);
}

void deleteStudent() {
    char targetID[6];
    int found = 0;

    printf("Enter the ID of the student to delete: ");
    scanf("%5s", targetID);

    for (int i = 0; i < numStudents; i++) {
        if (strcmp(students[i].id, targetID) == 0) {
            found = 1;

            printf("Deleting student with ID %s:\n", targetID);

            for (int j = i; j < numStudents - 1; j++) {
                students[j] = students[j + 1];
            }

            numStudents--;
            deleteStudentFromFile(targetID);

            printf("Student deleted successfully.\n");
            break;
        }
    }

    if (!found) {
        printf("Student with ID %s not found.\n", targetID);
    }
}
void displayStudents() {
    FILE *file = fopen("students.txt", "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("\n************************************************************\n");
    printf("Displaying Students:\n");
    printf("ID    | Name                           | Class      | Math  | Physics | Chemistry | Average\n");
    printf("--------------------------------------------------------------------------------------------\n");

    while (fscanf(file, "%5s,%30[^,],%10[^,],%f,%f,%f,%f\n",
                  students[numStudents].id,
                  students[numStudents].name,
                  students[numStudents].class,
                  &students[numStudents].math_score,
                  &students[numStudents].physics_score,
                  &students[numStudents].chemistry_score,
                  &students[numStudents].average_score) == 7) {
        printf("%-6s| %-30s| %-11s| %.2f  | %.2f    | %.2f      | %.2f\n",
               students[numStudents].id,
               students[numStudents].name,
               students[numStudents].class,
               students[numStudents].math_score,
               students[numStudents].physics_score,
               students[numStudents].chemistry_score,
               students[numStudents].average_score);

        numStudents++;
    }

    fclose(file);
}
struct Student students[MAX_STUDENTS];
int numStudent = 0;
int compareStudents(const void *a, const void *b) {
    const struct Student *studentA = (const struct Student *)a;
    const struct Student *studentB = (const struct Student *)b;

    return (int)(studentA->average_score - studentB->average_score);
}
void sortStudents(int order) {
    FILE *file = fopen("students.txt", "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    numStudents = 0;

    while (fscanf(file, "%5s,%30[^,],%10[^,],%f,%f,%f,%f\n",
                  students[numStudents].id,
                  students[numStudents].name,
                  students[numStudents].class,
                  &students[numStudents].math_score,
                  &students[numStudents].physics_score,
                  &students[numStudents].chemistry_score,
                  &students[numStudents].average_score) == 7) {
        numStudents++;
    }

    fclose(file);
    if (order == 1) {
        qsort(students, numStudents, sizeof(struct Student), compareStudents);
    } else if (order == 2) {
        qsort(students, numStudents, sizeof(struct Student), compareStudents);
        for (int i = 0, j = numStudents - 1; i < j; i++, j--) {
            struct Student temp = students[i];
            students[i] = students[j];
            students[j] = temp;
        }
    }

    // Display sorted students
    printf("\n************************************************************\n");
    printf("Sorted Students based on Average Score:\n");
    printf("ID    | Name                           | Class      | Math  | Physics | Chemistry | Average\n");
    printf("--------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < numStudents; i++) {
        printf("%-6s| %-30s| %-11s| %.2f  | %.2f    | %.2f      | %.2f\n",
               students[i].id,
               students[i].name,
               students[i].class,
               students[i].math_score,
               students[i].physics_score,
               students[i].chemistry_score,
               students[i].average_score);
    }
}
void searchByClass() {
    char targetClass[11];
    int found = 0;

    printf("Enter the class to search: ");
    scanf("%10s", targetClass);

    printf("\n************************************************************\n");
    printf("Search Results for Class %s:\n", targetClass);
    printf("ID    | Name                           | Class      | Math  | Physics | Chemistry | Average\n");
    printf("--------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < numStudents; i++) {
        if (strcmp(students[i].class, targetClass) == 0) {
            found = 1;
            printf("%-6s| %-30s| %-11s| %.2f  | %.2f    | %.2f      | %.2f\n",
                   students[i].id,
                   students[i].name,
                   students[i].class,
                   students[i].math_score,
                   students[i].physics_score,
                   students[i].chemistry_score,
                   students[i].average_score);
        }
    }

    if (!found) {
        printf("No students found in class %s.\n", targetClass);
    }
}
void searchByScoreRange() {
    float lowerLimit, upperLimit;
    int found = 0;

    printf("Enter the lower limit of the score range: ");
    scanf("%f", &lowerLimit);

    printf("Enter the upper limit of the score range: ");
    scanf("%f", &upperLimit);

    printf("\n************************************************************\n");
    printf("Search Results for Score Range (%.2f - %.2f):\n", lowerLimit, upperLimit);
    printf("ID    | Name                           | Class      | Math  | Physics | Chemistry | Average\n");
    printf("--------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < numStudents; i++) {
        if (students[i].average_score >= lowerLimit && students[i].average_score <= upperLimit) {
            found = 1;
            printf("%-6s| %-30s| %-11s| %.2f  | %.2f    | %.2f      | %.2f\n",
                   students[i].id,
                   students[i].name,
                   students[i].class,
                   students[i].math_score,
                   students[i].physics_score,
                   students[i].chemistry_score,
                   students[i].average_score);
        }
    }

    if (!found) {
        printf("No students found in the specified score range.\n");
    }
}

