#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define MAX_NAME 51
#define MAX_COMMAND 2048

typedef struct FreeBlock {
    int index;
    struct FreeBlock *next;
    struct FreeBlock *prev;
} FreeBlock;


typedef struct FileNode {
    char name[MAX_NAME];
    int isDirectory;
    struct FileNode *parent;
    struct FileNode *next;
    struct FileNode *prev;
    int *blockPointers;
    int blockCount;
    size_t fileSize;
} FileNode;

char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];

FreeBlock *freeHead = NULL;
FreeBlock *freeTail = NULL;

FileNode *root = NULL;
FileNode *cwd = NULL;

int usedBlocks = 0;

void initializeFreeBlocks() {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        FreeBlock *newBlock = malloc(sizeof(FreeBlock));
        if (!newBlock) {
            printf("Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
        newBlock->index = i;
        newBlock->next = NULL;
        newBlock->prev = freeTail;
        if (freeHead == NULL) {
            freeHead = newBlock;
        } else {
            freeTail->next = newBlock;
        }
        freeTail = newBlock;
    }
}

int allocateBlock() {
    if (freeHead == NULL)
        return -1;
    FreeBlock *temp = freeHead;
    int index = temp->index;
    freeHead = freeHead->next;
    if (freeHead)
        freeHead->prev = NULL;
    else
        freeTail = NULL;
    free(temp);
    usedBlocks++;
    return index;
}

void freeBlock(int index) {
    FreeBlock *newBlock = malloc(sizeof(FreeBlock));
    if (!newBlock) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newBlock->index = index;
    newBlock->next = NULL;
    newBlock->prev = freeTail;
    if (freeTail)
        freeTail->next = newBlock;
    else
        freeHead = newBlock;
    freeTail = newBlock;
    usedBlocks--;
}

FileNode *createNode(const char *name, int isDirectory) {
    if (strlen(name) >= MAX_NAME)
        return NULL;
    FileNode *node = calloc(1, sizeof(FileNode));
    if (!node)
        return NULL;
    strcpy(node->name, name);
    node->isDirectory = isDirectory;
    node->blockPointers = NULL;
    node->blockCount = 0;
    node->fileSize = 0
    node->next = node;
    node->prev = node;
    return node;
}

void insertNode(FileNode *parent, FileNode *node) {
    if (!parent->next) {
        parent->next = node;
        parent->prev = node;
    } else {
        FileNode *tail = parent->prev;
        tail->next = node;
        node->prev = tail;
        node->next = parent->next;
        parent->next->prev = node;
        parent->prev = node;
    }
    node->parent = parent;
}

FileNode *findNode(FileNode *parent, const char *name) {
    if (!parent || !parent->next)
        return NULL;
    FileNode *current = parent->next;
    do {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->next;
    } while (current != parent->next);
    return NULL;
}

void removeNode(FileNode *node) {
    FileNode *parent = node->parent;
    if (node->next == node) {
        parent->next = NULL;
        parent->prev = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        if (parent->next == node)
            parent->next = node->next;

        if (parent->prev == node)
            parent->prev = node->prev;
    }
    node->next = node;
    node->prev = node;
}

void mkdirCommand(char *name) {
    if (!name || strlen(name) == 0 || strlen(name) >= MAX_NAME) {
        printf("Invalid directory name.\n");
        return;
    }
    if (strchr(name, '/') || strcmp(name, ".") == 0 ||
        strcmp(name, "..") == 0) {
        printf("Invalid directory name.\n");
        return;
    }
    if (findNode(cwd, name)) {
        printf("An entry with this name already exists.\n");
        return;
    }
    FileNode *dir = createNode(name, 1);
    if (!dir) {
        printf("Memory allocation failed.\n");
        return;
    }
    insertNode(cwd, dir);
    printf("Directory '%s' created successfully.\n", name);
}

void createCommand(char *name) {
    if (!name || strlen(name) == 0 || strlen(name) >= MAX_NAME) {
        printf("Invalid file name.\n");
        return;
    }
    if (strchr(name, '/') || strcmp(name, ".") == 0 ||
        strcmp(name, "..") == 0) {
        printf("Invalid file name.\n");
        return;
    }
    if (findNode(cwd, name)) {
        printf("An entry with this name already exists.\n");
        return;
    }
    FileNode *file = createNode(name, 0);
    if (!file) {
        printf("Memory allocation failed.\n");
        return;
    }
    insertNode(cwd, file);
    printf("File '%s' created successfully.\n", name);
}

void writeCommand(char *name, char *content) {
    FileNode *file = findNode(cwd, name);
    if (!file) {
        printf("File not found.\n");
        return;
    }
    if (file->isDirectory) {
        printf("Cannot write to a directory.\n");
        return;
    }
    size_t size = strlen(content);
    int requiredBlocks = (int)((size + BLOCK_SIZE - 1) / BLOCK_SIZE);
    int available = 0;
    FreeBlock *temp = freeHead
    while (temp) {
        available++;
        temp = temp->next;
    }
    if (requiredBlocks > available) {
        printf("Disk full! Not enough free blocks.\n");
        return;
    }
    int *newPointers = NULL;
    if (requiredBlocks > 0) {
        newPointers = malloc(requiredBlocks * sizeof(int));
        if (!newPointers) {
            printf("Memory allocation failed.\n");
            return;
        }
    }
    for (int i = 0; i < requiredBlocks; i++) {
        newPointers[i] = allocateBlock();
    }
    for (int i = 0; i < file->blockCount; i++) {
        freeBlock(file->blockPointers[i]);
    }
    free(file->blockPointers);
    file->blockPointers = newPointers;
    file->blockCount = requiredBlocks;
    file->fileSize = size;
    size_t offset = 0;
    for (int i = 0; i < requiredBlocks; i++) {
        size_t remaining = size - offset;
        size_t bytesToWrite =
            remaining < BLOCK_SIZE ? remaining : BLOCK_SIZE
        memcpy(
            virtualDisk[file->blockPointers[i]],
            content + offset,
            bytesToWrite
        );
        offset += bytesToWrite;
    }
    printf("Data written successfully (size=%zu bytes).\n", size);
}

void readCommand(char *name) {
    FileNode *file = findNode(cwd, name);
    if (!file) {
        printf("File not found.\n");
        return;
    }
    if (file->isDirectory) {
        printf("Cannot read a directory.\n");
        return;
    }
    if (file->fileSize == 0) {
        printf("(empty file)\n");
        return;
    }
    size_t remaining = file->fileSize;
    for (int i = 0; i < file->blockCount; i++) {
        size_t bytesToRead =
            remaining < BLOCK_SIZE ? remaining : BLOCK_SIZE;
        fwrite(
            virtualDisk[file->blockPointers[i]],
            1,
            bytesToRead,
            stdout
        );
        remaining -= bytesToRead;
    }
    printf("\n");
}

void deleteCommand(char *name) {
    FileNode *file = findNode(cwd, name);
    if (!file) {
        printf("File not found.\n");
        return;
    }
    if (file->isDirectory) {
        printf("Use rmdir to remove a directory.\n");
        return;
    }
    for (int i = 0; i < file->blockCount; i++) {
        freeBlock(file->blockPointers[i]);
    }
    free(file->blockPointers);
    removeNode(file);
    free(file);
    printf("File deleted successfully.\n");
}

void rmdirCommand(char *name) {
    FileNode *dir = findNode(cwd, name);
    if (!dir) {
        printf("Directory not found.\n");
        return;
    }
    if (!dir->isDirectory) {
        printf("Not a directory.\n");
        return;
    }
    if (dir->next != NULL) {
        printf("Directory is not empty.\n");
        return;
    }
    removeNode(dir);
    free(dir);
    printf("Directory removed successfully.\n");
}

void lsCommand() {
    if (!cwd->next) {
        printf("(empty)\n");
        return;
    }
    FileNode *current = cwd->next;
    do {
        if (current->isDirectory)
            printf("[DIR]  %s/\n", current->name);
        else
            printf("[FILE] %s\n", current->name);
        current = current->next;
    } while (current != cwd->next);
}

void cdCommand(char *name) {
    if (strcmp(name, "/") == 0) {
        cwd = root;
        printf("Moved to /\n");
        return;
    }
    if (strcmp(name, "..") == 0) {
        if (cwd->parent)
            cwd = cwd->parent;

        printf("Moved to parent directory.\n");
        return;
    }
    if (strcmp(name, ".") == 0)
        return;
    FileNode *dir = findNode(cwd, name);
    if (!dir) {
        printf("Directory not found.\n");
        return;
    }
    if (!dir->isDirectory) {
        printf("Not a directory.\n");
        return;
    }
    cwd = dir;
    printf("Moved to directory '%s'.\n", cwd->name);
}

void pwdCommand() {
    char *parts[100];
    int count = 0;
    FileNode *current = cwd
    while (current && current != root && count < 100) {
        parts[count++] = current->name;
        current = current->parent;
    }
    printf("/");
    for (int i = count - 1; i >= 0; i--) {
        printf("%s", parts[i]);
        if (i > 0)
            printf("/");
    }
    printf("\n");
}

void dfCommand() {
    int freeBlocks = NUM_BLOCKS - usedBlocks;
    float usage = (float)usedBlocks / NUM_BLOCKS * 100.0f;
    printf("Total Blocks: %d\n", NUM_BLOCKS);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", freeBlocks);
    printf("Disk Usage: %.2f%%\n", usage);
}

void freeDirectory(FileNode *dir) {
    if (!dir || !dir->next)
        return;
    FileNode *current = dir->next;
    while (current != dir->next) {
        FileNode *next = current->next;
        if (current->isDirectory) {
            freeDirectory(current);
        } else {
            for (int i = 0; i < current->blockCount; i++) {
                freeBlock(current->blockPointers[i]);
            }
            free(current->blockPointers);
        }
        free(current);
        current = next;
    }
    dir->next = NULL;
    dir->prev = NULL;
}

void cleanup() {
    if (root)
        freeDirectory(root);
    FreeBlock *current = freeHead;
    while (current) {
        FreeBlock *next = current->next;
        free(current);
        current = next;
    }
    freeHead = NULL;
    freeTail = NULL;
    free(root);
    root = NULL;
    cwd = NULL;
    printf("Memory released. Exiting program...\n");
}

void executeCommand(char *input) {
    char *command = strtok(input, " \t\n");
    if (!command)
        return;
    char *argument = strtok(NULL, " \t\n");
    if (strcmp(command, "mkdir") == 0) {
        if (argument)
            mkdirCommand(argument);
        else
            printf("Usage: mkdir dirname\n");
    }
    else if (strcmp(command, "create") == 0) {
        if (argument)
            createCommand(argument);
        else
            printf("Usage: create filename\n");
    }
    else if (strcmp(command, "write") == 0) {
        if (!argument) {
            printf("Usage: write filename \"content\"\n");
            return;
        }
        char *content = strtok(NULL, "\n");
        if (!content) {
            printf("Usage: write filename \"content\"\n");
            return;
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
        writeCommand(argument, content);
    }
    else if (strcmp(command, "read") == 0) {
        if (argument)
            readCommand(argument);
        else
            printf("Usage: read filename\n");
    }
    else if (strcmp(command, "delete") == 0) {
        if (argument)
            deleteCommand(argument);
        else
            printf("Usage: delete filename\n");
    }
    else if (strcmp(command, "rmdir") == 0) {
        if (argument)
            rmdirCommand(argument);
        else
            printf("Usage: rmdir dirname\n");
    }
    else if (strcmp(command, "ls") == 0) {
        lsCommand();
    }
    else if (strcmp(command, "cd") == 0) {
        if (argument)
            cdCommand(argument);
        else
            printf("Usage: cd dirname\n");
    }
    else if (strcmp(command, "pwd") == 0) {
        pwdCommand();
    }
    else if (strcmp(command, "df") == 0) {
        dfCommand();
    }
    else if (strcmp(command, "exit") == 0) {
        cleanup();
        exit(0);
    }
    else {
        printf("Unknown command: %s\n", command);
    }
}

int main() {
    printf("Initializing Virtual File System...\n");
    initializeFreeBlocks();
    root = createNode("/", 1);
    if (!root) {
        printf("Root allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    root->parent = NULL;
    cwd = root
    printf("Compact VFS - ready. Type 'exit' to quit.\n");
    char input[MAX_COMMAND];
    while (1) {
        pwdCommand();
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) {
            cleanup();
            break;
        }
        executeCommand(input);
    }
    return 0;
}