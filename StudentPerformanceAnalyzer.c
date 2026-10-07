#include <stdio.h>

typedef struct Student {
    int rollNumber;
    char name[50];
    float marks1;
    float marks2;
    float marks3;
}Student;

float totalMarks(Student s) {
    return s.marks1 + s.marks2 + s.marks3;
}

float averageMarks(float total) {
    return total / 3.0;
}

char Grade(float average) {
    if (average >= 85)
        return 'A';
    else if (average >= 70)
        return 'B';
    else if (average >= 50)
        return 'C';
    else if (average >= 35)
        return 'D';
    else
        return 'F';
}

void Stars(char grade) {
    int count = 0;
    if (grade == 'A')
        count = 5;
    else if (grade == 'B')
        count = 4;
    else if (grade == 'C')
        count = 3;
    else if (grade == 'D')
        count = 2;
    for (int i = 0; i < count; i++) {
        printf("*");
    }
}

void rollNumbers(Student students[], int n, int i) {
    if (i == n)
        return;
    printf("%d", students[i].rollNumber);
    if (i < n - 1)
        printf(" ");
    rollNumbers(students, n, i + 1);
}

int main() {
    int n;
    printf("Enter number of students: ");
    scanf("%d", &n);
    if(n < 1 || n > 100) {
        printf("Invalid Input.\n");
        return 1;
    }
    Student students[n];
    for (int i = 0; i < n; i++) {
        printf("\nEnter details for student %d:\n", i + 1);
        scanf("%d %49s %f %f %f", &students[i].rollNumber, students[i].name, &students[i].marks1, &students[i].marks2, &students[i].marks3);
    }
    printf("\n");
    for (int i = 0; i < n; i++) {
        float total = totalMarks(students[i]);
        float average = averageMarks(total);
        char grade = Grade(average);
        printf("Roll: %d\n", students[i].rollNumber);
        printf("Name: %s\n", students[i].name);
        printf("Total: %.0f\n", total);
        printf("Average: %.2f\n", average);
        printf("Grade: %c\n", grade);
        if (average < 35) {
            continue;
        }
        printf("Performance: ");
        Stars(grade);
        printf("\n");
        printf("\n");
    }
    printf("List of Roll Numbers : ");
    rollNumbers(students, n, 0);
    printf("\n");
    return 0;
}
