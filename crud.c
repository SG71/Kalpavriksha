#include <stdio.h>
#include <string.h>

#define File_Name "users.txt"

typedef struct Users {
    int id;
    char name[100];
    int age;
} User;

void create() {
    FILE *fp = fopen(File_Name, "a");
    fclose(fp);
}

int check(int ID) {
    User user;
    FILE *fp = fopen(File_Name, "r");
    while (fscanf(fp, "%d\t%99[^\t]\t%d", &user.id, user.name, &user.age) == 3) {
        if (user.id == ID) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void add() {
    User user;
    printf("Enter User ID: ");
    scanf("%d", &user.id);
    if (check(user.id)) {
        printf("User ID %d Already Exists.\n", user.id);
        return;
    }
    while (getchar() != '\n');
    printf("Enter User Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strcspn(user.name, "\n")] = '\0';
    printf("Enter User Age: ");
    scanf("%d", &user.age);
    FILE *fp = fopen(File_Name, "a");
    fprintf(fp, "%d\t%s\t%d\n", user.id, user.name, user.age);
    fclose(fp);
    printf("User successfully added.\n");
}

void read() {
    User user;
    int found = 0;
    FILE *fp = fopen(File_Name, "r");
    printf("\nUser Records\n");
    while (fscanf(fp, "%d\t%99[^\t]\t%d", &user.id, user.name, &user.age) == 3) {
        found = 1;
        printf("ID   : %d\n", user.id);
        printf("Name : %s\n", user.name);
        printf("Age  : %d\n", user.age);
        printf("\n");
    }
    if (found == 0) {
        printf("No User Records Found.\n");
    }
    fclose(fp);
}

void update() {
    User user;
    int ID;
    int found = 0;
    FILE *fp = fopen(File_Name, "r");
    FILE *temp = fopen("temp.txt", "w");
    printf("Enter User ID To Update: ");
    scanf("%d", &ID);
    while (getchar() != '\n');
    while (fscanf(fp, "%d\t%99[^\t]\t%d", &user.id, user.name, &user.age) == 3) {
        if (user.id == ID) {
            found = 1;
            printf("Enter New Name: ");
            fgets(user.name, sizeof(user.name), stdin);
            user.name[strcspn(user.name, "\n")] = '\0';
            printf("Enter New Age: ");
            scanf("%d", &user.age);
        }
        fprintf(temp, "%d\t%s\t%d\n", user.id, user.name, user.age);
    }
    fclose(fp);
    fclose(temp);
    remove(File_Name);
    rename("temp.txt", File_Name);
    if (found) {
        printf("User Updated Successfully!\n");
    } else {
        printf("User ID Not Found!\n");
    }
}

void delete() {
    User user;
    int ID;
    int found = 0;
    FILE *fp = fopen(File_Name, "r");
    FILE *temp = fopen("temp.txt", "w");
    printf("Enter User ID To Delete: ");
    scanf("%d", &ID);
    while (fscanf(fp, "%d\t%99[^\t]\t%d", &user.id, user.name, &user.age) == 3) {
        if (user.id == ID) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d\t%s\t%d\n", user.id, user.name, user.age);
    }
    fclose(fp);
    fclose(temp);
    remove(File_Name);
    rename("temp.txt", File_Name);
    if (found) {
        printf("User Deleted Successfully!\n");
    } else {
        printf("User ID Not Found!\n");
    }
}

int main() {
    int choice;
    create();
    do {
        printf("\nUSER MANAGEMENT\n");
        printf("1. Add User\n");
        printf("2. Display Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("\n");
        printf("Enter Your Choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                add();
                break;
            case 2:
                read();
                break;
            case 3:
                update();
                break;
            case 4:
                delete();
                break;
            case 5:
                printf("Program Exited.\n");
                break;
            default:
                printf("Invalid Choice!\n");
        }
    } while (choice != 5);
    return 0;
}
