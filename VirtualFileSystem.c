#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BLOCK_SIZE 512
#define TOTAL 1024
#define MAX_NAME 51
#define MAX_INPUT 2048

typedef struct Free {
    int blockNo;
    struct Free *next;
    struct Free *prev;
} Free;

typedef struct File {
    char name[MAX_NAME];
    int isDirectory;
    size_t size;
    int *blocks;
    int blockCount;
    struct File *parent;
    struct File *children;
    struct File *next;
    struct File *prev;
} File;

char disk[TOTAL][BLOCK_SIZE];

Free *freeHead = NULL;
Free *freeTail = NULL;

File *root = NULL;
File *currentDir = NULL;

int usedBlocks = 0;

void initialize() {
    for (int i = 0; i < TOTAL; i++) {
        Free *block = malloc(sizeof(Free));
        block->blockNo = i;
        block->next = NULL;
        block->prev = freeTail;
        if (freeHead == NULL)
            freeHead = block;
        else
            freeTail->next = block;
        freeTail = block;
    }
}

int allocate() {
    if (freeHead == NULL)
        return -1;
    Free *temp = freeHead;
    int blockNo = temp->blockNo;
    freeHead = freeHead->next;
    if (freeHead)
        freeHead->prev = NULL;
    else
        freeTail = NULL;
    free(temp);
    usedBlocks++;
    return blockNo;
}

void release(int blockNo) {
    Free *block = malloc(sizeof(Free));
    block->blockNo = blockNo;
    block->next = NULL;
    block->prev = freeTail;
    if (freeTail)
        freeTail->next = block;
    else
        freeHead = block;
    freeTail = block;
    usedBlocks--;
}

File *create(char *name, int isDirectory) {
    File *newFile = calloc(1, sizeof(File));
    if (!newFile)
        return NULL;
    strcpy(newFile->name, name);
    newFile->isDirectory = isDirectory;
    newFile->size = 0;
    newFile->blocks = NULL;
    newFile->blockCount = 0;
    newFile->children = NULL;
    newFile->parent = NULL;
    newFile->next = newFile;
    newFile->prev = newFile;
    return newFile;
}

void add(File *parent, File *newFile) {
    if (parent->children == NULL) {
        parent->children = newFile;
    } else {
        File *last = parent->children->prev;
        last->next = newFile;
        newFile->prev = last;
        newFile->next = parent->children;
        parent->children->prev = newFile;
    }
    newFile->parent = parent;
}

File *find(File *parent, char *name) {
    if (parent == NULL || parent->children == NULL)
        return NULL;
    File *temp = parent->children;
    do {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->next;
    } while (temp != parent->children);
    return NULL;
}

void remove(File *file) {
    File *parent = file->parent;
    if (file->next == file) {
        parent->children = NULL;
    } else {
        file->prev->next = file->next;
        file->next->prev = file->prev;
        if (parent->children == file)
            parent->children = file->next;
    }
    file->next = file;
    file->prev = file;
}

void mkdir(char *name) {
    if (!name || strlen(name) == 0 || strlen(name) >= MAX_NAME || strchr(name, '/') || strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
        printf("Invalid directory name.\n");
        return;
    }
    if (find(currentDir, name)) {
        printf("An entry with this name already exists.\n");
        return;
    }
    File *dir = create(name, 1);
    add(currentDir, dir);
    printf("Directory '%s' created successfully.\n", name);
}

void create(char *name) {
    if (!name || strlen(name) == 0 || strlen(name) >= MAX_NAME || strchr(name, '/') || strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
        printf("Invalid file name.\n");
        return;
    }
    if (find(currentDir, name)) {
        printf("An entry with this name already exists.\n");
        return;
    }
    File *file = create(name, 0);
    if (!file) {
        printf("Memory allocation failed.\n");
        return;
    }
    add(currentDir, file);
    printf("File '%s' created successfully.\n", name);
}

void write(char *name, char *content) {
    File *file = find(currentDir, name);
    if (!file) {
        printf("File not found.\n");
        return;
    }
    if (file->isDirectory) {
        printf("Cannot write to a directory.\n");
        return;
    }
    size_t size = strlen(content);
    int needed = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int available = TOTAL - usedBlocks;
    if (needed > available) {
        printf("Disk full! Not enough free blocks.\n");
        return;
    }
    int *newBlocks = NULL;
    if (needed > 0) {
        newBlocks = malloc(needed * sizeof(int));
    }
    for (int i = 0; i < needed; i++) {
        newBlocks[i] = allocate();
    }
    for (int i = 0; i < file->blockCount; i++) {
        release(file->blocks[i]);
    }
    free(file->blocks);
    file->blocks = newBlocks;
    file->blockCount = needed;
    file->size = size;
    size_t position = 0;
    for (int i = 0; i < needed; i++) {
        for (int j = 0; j < BLOCK_SIZE && position < size; j++) {
            disk[file->blocks[i]][j] = content[position];
            position++;
        }
    }
    printf("Data written successfully (%zu bytes).\n", size);
}

void read(char *name) {
    File *file = find(currentDir, name);
    if (!file) {
        printf("File not found.\n");
        return;
    }
    if (file->isDirectory) {
        printf("Cannot read a directory.\n");
        return;
    }
    if (file->size == 0) {
        printf("(empty file)\n");
        return;
    }
    size_t remaining = file->size
    for (int i = 0; i < file->blockCount; i++) {
        for (int j = 0; j < BLOCK_SIZE && remaining > 0; j++) {
            putchar(disk[file->blocks[i]][j]);
            remaining--;
        }
    }
    printf("\n");
}

void delete(char *name) {
    File *file = find(currentDir, name);
    if (!file) {
        printf("File not found.\n");
        return;
    }
    if (file->isDirectory) {
        printf("Use rmdir to remove a directory.\n");
        return;
    }
    for (int i = 0; i < file->blockCount; i++) {
        release(file->blocks[i]);
    }
    free(file->blocks);
    remove(file);
    free(file);
    printf("File deleted successfully.\n");
}

void rmdir(char *name) {
    File *dir = find(currentDir, name);
    if (!dir) {
        printf("Directory not found.\n");
        return;
    }
    if (!dir->isDirectory) {
        printf("Not a directory.\n");
        return;
    }
    if (dir->children != NULL) {
        printf("Directory is not empty.\n");
        return;
    }
    remove(dir);
    free(dir);
    printf("Directory removed successfully.\n");
}

void ls() {
    if (currentDir->children == NULL) {
        printf("(empty)\n");
        return;
    }
    File *temp = currentDir->children;
    do {
        if (temp->isDirectory)
            printf("[DIR]  %s/\n", temp->name);
        else
            printf("[FILE] %s\n", temp->name);
        temp = temp->next;
    } while (temp != currentDir->children);
}

void cd(char *name) {
    if (strcmp(name, "/") == 0) {
        currentDir = root;
        printf("Moved to /\n");
        return;
    }
    if (strcmp(name, "..") == 0) {
        if (currentDir->parent)
            currentDir = currentDir->parent;

        printf("Moved to parent directory.\n");
        return;
    }
    if (strcmp(name, ".") == 0)
        return;
    File *dir = find(currentDir, name);
    if (!dir) {
        printf("Directory not found.\n");
        return;
    }
    if (!dir->isDirectory) {
        printf("Not a directory.\n");
        return;
    }
    currentDir = dir;
    printf("Moved to directory '%s'.\n", currentDir->name);
}

void pwd() {
    char *path[100];
    int count = 0;
    File *temp = currentDir;
    while (temp && temp != root && count < 100) {
        path[count++] = temp->name;
        temp = temp->parent;
    }
    printf("/");
    for (int i = count - 1; i >= 0; i--) {
        printf("%s", path[i]);
        if (i > 0)
            printf("/");
    }
    printf("\n");
}

void df() {
    int Frees = TOTAL - usedBlocks;
    float usage = (float)usedBlocks / TOTAL * 100;
    printf("Total Blocks: %d\n", TOTAL);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", Frees);
    printf("Disk Usage: %.2f%%\n", usage);
}

void freeDirectory(File *dir) {
    if (!dir)
        return;
    while (dir->children != NULL) {
        File *child = dir->children;
        remove(child);
        if (child->isDirectory) {
            freeDirectory(child);
        } else {
            for (int i = 0; i < child->blockCount; i++) {
                release(child->blocks[i]);
            }
            free(child->blocks);
        }
        free(child);
    }
}

void exit() {
    if (root) {
        freeDirectory(root);
        free(root);
        root = NULL;
        currentDir = NULL;
    }
    Free *temp = freeHead;
    while (temp) {
        Free *next = temp->next;
        free(temp);
        temp = next;
    }
    freeHead = NULL;
    freeTail = NULL;
    printf("Memory released. Exiting program...\n");
}

int main() {
    char input[MAX_INPUT];
    printf("Initializing Virtual File System...\n");
    initialize();
    root = create("/", 1);
    if (!root) {
        printf("Root allocation failed.\n");
        exit();
        return 1;
    }
    currentDir = root;
    printf("Virtual File System is ready.\n");
    printf("Type 'exit' to quit.\n");
    while (1) {
        pwd();
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) {
            exit();
            break;
        }
        char *command = strtok(input, " \t\n");
        char *argument = strtok(NULL, " \t\n");
        if (!command)
            continue;
        if (strcmp(command, "mkdir") == 0) {
            if (argument)
                mkdir(argument);
            else
                printf("Usage: mkdir dirname\n");
        }
        else if (strcmp(command, "create") == 0) {
            if (argument)
                create(argument);
            else
                printf("Usage: create filename\n");
        }
        else if (strcmp(command, "write") == 0) {
            if (!argument) {
                printf("Usage: write filename \"content\"\n");
                continue;
            }
            char *content = strtok(NULL, "\n");
            if (!content) {
                printf("Usage: write filename \"content\"\n");
                continue;
            }
            while (isspace((unsigned char)*content))
                content++;
            size_t len = strlen(content);
            if (len >= 2 &&
                content[0] == '"' &&
                content[len - 1] == '"') {
                content[len - 1] = '\0';
                content++;
            }
            write(argument, content);
        }
        else if (strcmp(command, "read") == 0) {
            if (argument)
                read(argument);
            else
                printf("Usage: read filename\n");
        }
        else if (strcmp(command, "delete") == 0) {
            if (argument)
                delete(argument);
            else
                printf("Usage: delete filename\n");
        }
        else if (strcmp(command, "rmdir") == 0) {
            if (argument)
                rmdir(argument);
            else
                printf("Usage: rmdir dirname\n");
        }
        else if (strcmp(command, "ls") == 0) {
            ls();
        }
        else if (strcmp(command, "cd") == 0) {
            if (argument)
                cd(argument);
            else
                printf("Usage: cd dirname\n");
        }
        else if (strcmp(command, "pwd") == 0) {
            pwd();
        }
        else if (strcmp(command, "df") == 0) {
            df();
        }
        else if (strcmp(command, "exit") == 0) {
            exit();
            break;
        }
        else {
            printf("Unknown command: %s\n", command);
        }
    }
    return 0;
}