#include <stdio.h>
#include <string.h>

#define MAX_EMPLOYEES 100

typedef struct Employee {
    int id;
    char name[50];
    char designation[50];
    int manager;
    int reports[MAX_EMPLOYEES];
    int report_count;
    int active;
} Employee;

Employee employees[MAX_EMPLOYEES];
int count = 0;

int ceo_id = -1;

Employee *checkID(int id) {
    for (int i = 0; i < count; i++) {
        if (employees[i].active && employees[i].id == id)
            return &employees[i];
    }
    return NULL;
}

void addEmployeeToManager(Employee *manager, int emp_id) {
    if (manager == NULL)
        return;
    if (manager->report_count < MAX_EMPLOYEES)
        manager->reports[manager->report_count++] = emp_id;
}

void add() {
    int id, manager_id;
    Employee *manager = NULL;
    if (count >= MAX_EMPLOYEES) {
        printf("Employee storage is full.\n");
        return;
    }
    printf("Enter Employee ID: ");
    scanf("%d", &id);
    if (checkID(id) != NULL) {
        printf("Employee ID already exists.\n");
        return;
    }
    printf("Enter Reporting Manager ID: ");
    scanf("%d", &manager_id);
    if (manager_id == 0) {
        if (ceo_id != -1) {
            printf("A top-level employee already exists (CEO ID: %d).\n", ceo_id);
            return;
        }
    } else {
        if (ceo_id == -1) {
            printf("Create the CEO first.\n");
            return;
        }
        manager = checkID(manager_id);
        if (manager == NULL) {
            printf("Reporting manager not found.\n");
            return;
        }
        if (manager->report_count >= MAX_EMPLOYEES) {
            printf("Manager's report list is full.\n");
            return;
        }
    }
    Employee *emp = &employees[count];
    printf("Enter Name: ");
    scanf(" %49[^\n]", emp->name);
    printf("Enter Designation: ");
    scanf(" %49[^\n]", emp->designation);
    emp->id = id;
    emp->manager = manager_id;
    emp->report_count = 0;
    emp->active = 1;
    count++;
    if (manager_id == 0) {
        ceo_id = id;
        printf("CEO created successfully.\n");
    } else {
        addEmployeeToManager(manager, id);
        printf("Employee added successfully.\n");
    }
}

void removeEmployee() {
    int id, i;
    Employee *emp, *manager, *target = NULL;
    int fewest = MAX_EMPLOYEES + 1;
    printf("Enter Employee ID to remove: ");
    if (scanf("%d", &id) != 1)
        return;
    emp = checkID(id);
    if (emp == NULL) {
        printf("Employee not found.\n");
        return;
    }
    if (emp->id == ceo_id) {
        printf("The CEO cannot be removed.\n");
        return;
    }
    manager = checkID(emp->manager);
    if (manager == NULL) {
        printf("Reporting manager not found.\n");
        return;
    }
    for (i = 0; i < manager->report_count; i++) {
        int sibling_id = manager->reports[i];
        if (sibling_id == emp->id)
            continue;
        Employee *sibling = checkID(sibling_id);
        if (sibling != NULL && sibling->report_count < fewest) {
            fewest = sibling->report_count;
            target = sibling;
        }
    }
    if (target == NULL)
        target = manager;
    for (i = 0; i < emp->report_count; i++) {
        Employee *report = checkID(emp->reports[i]);
        if (report != NULL) {
            report->manager = target->id;
            addEmployeeToManager(target, report->id);
        }
    }
    for (i = 0; i < manager->report_count; i++) {
        if (manager->reports[i] == emp->id) {
            for (int j = i; j < manager->report_count - 1; j++) {
                manager->reports[j] = manager->reports[j + 1];
            }
            manager->report_count--;
            break;
        }
    }
    emp->active = 0;
    emp->manager = 0;
    emp->report_count = 0;
    printf("Employee %d removed successfully.\n", id);
    printf("Direct reports transferred to %s (ID %d).\n", target->name, target->id);
}

int employeeCountUnderAManager(Employee *emp) {
    int total = 0;
    if (emp == NULL)
        return 0;
    for (int i = 0; i < emp->report_count; i++) {
        Employee *report = checkID(emp->reports[i]);
        if (report != NULL)
            total += 1 + employeeCountUnderAManager(report);
    }
    return total;
}

void hierarchy(Employee *emp, int level, int *total) {
    if (emp == NULL)
        return;
    for (int i = 0; i < level; i++)
        printf("    ");
    printf("%d - %s - %s\n", emp->id, emp->name, emp->designation);
    for (int i = 0; i < emp->report_count; i++) {
        Employee *report = checkID(emp->reports[i]);
        if (report != NULL) {
            (*total)++;
            hierarchy(report, level + 1, total);
        }
    }
}

void displayEmployee() {
    int id, total = 0;
    printf("Enter Employee ID: ");
    scanf("%d", &id);
    Employee *emp = checkID(id);
    if (emp == NULL) {
        printf("Employee not found.\n");
        return;
    }
    hierarchy(emp, 0, &total);
    printf("Total Employees Under %d: %d\n", id, total);
}

void displayComplete(void) {
    int total = 1;
    printf("\n===== ORGANIZATION HIERARCHY =====\n");
    if (ceo_id == -1) {
        printf("No CEO exists. Create the top-level employee first.\n");
        return;
    }
    Employee *ceo = checkID(ceo_id);
    if (ceo == NULL)
        return;
    hierarchy(ceo, 0, &total);
    printf("Total Employees: %d\n", total);
}

int main(void) {
    int choice;
    do {
        printf("\n===== EMPLOYEE HIERARCHY MANAGEMENT =====\n");
        printf("1. Add Employee\n");
        printf("2. Remove Employee\n");
        printf("3. Display Employee Hierarchy\n");
        printf("4. Display Complete Hierarchy\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
            break;
        switch (choice) {
            case 1:
                add();
                break;
            case 2:
                removeEmployee();
                break;
            case 3:
                displayEmployee();
                break;
            case 4:
                displayComplete();
                break;
            case 5:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);
    return 0;
}