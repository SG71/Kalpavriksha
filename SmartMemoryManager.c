#include<stdio.h>
#include<stdlib.h>

int id = 101;
size_t number_of_blocks = 0;
size_t total_memory = 0;

typedef struct{
    size_t start;
    size_t size;
    int id;
    int is_free;
}Block;

enum Strategy {
    FIRST_FIT = 1,
    BEST_FIT,
    WORST_FIT
};

unsigned char *memory = NULL;
Block *blocks = NULL;
size_t capacity = 0;
enum Strategy current_strategy = FIRST_FIT;

int initialize_memory(size_t pool_size) {
    memory = malloc(pool_size * sizeof(unsigned char));
    if(memory == NULL) return 0;
    capacity = 1;
    number_of_blocks = 1;
    blocks = malloc(capacity * sizeof(Block));
    if(blocks == NULL){
        free(memory);
        memory = NULL;
        return 0;
    }
    blocks[0].start = 0;
    blocks[0].size = pool_size;
    blocks[0].id = 0;
    blocks[0].is_free = 1;
    printf("Memory initialized successfully!\n");
    printf("Memory Pool: %zu bytes\n", pool_size);
    printf("[Free: %zu bytes, Start: 0]\n", pool_size);
    total_memory = pool_size;
    return 1;
}

void display_memory_map(void) {
    printf("Memory Map (%zu bytes):\n", total_memory);
    for(size_t i=0;i<number_of_blocks;i++) {
        if(blocks[i].is_free == 0) {
            printf("[Allocated #%d: %zu bytes, Start: %zu]\n",blocks[i].id, blocks[i].size, blocks[i].start);
        } else {
            printf("[Free: %zu bytes, Start: %zu]\n", blocks[i].size, blocks[i].start);
        }
    }
}

int first_fit(size_t requested_size) {
    for(size_t i=0;i<number_of_blocks;i++) {
        if(blocks[i].is_free == 1) {
            if(blocks[i].size >= requested_size) return i;
        }
    }
    return -1;
}
int best_fit(size_t requested_size) {
    size_t curr = 0;
    int index = -1;
    for(size_t i=0;i<number_of_blocks;i++) {
        if(blocks[i].is_free == 1) {
            if(blocks[i].size >= requested_size) {
                if(curr == 0) {
                    curr = blocks[i].size;
                    index = i;
                } else {
                    if(curr > blocks[i].size) {
                        curr = blocks[i].size;
                        index = i;
                    }
                }
            }
        }
    }
    return index;
}
int worst_fit(size_t requested_size) {
    size_t curr = 0;
    int index = -1;
    for(size_t i=0;i<number_of_blocks;i++) {
        if(blocks[i].is_free == 1) {
            if(blocks[i].size >= requested_size) {
                if(curr == 0) {
                    curr = blocks[i].size;
                    index = i;
                } else {
                    if(curr < blocks[i].size) {
                        curr = blocks[i].size;
                        index = i;
                    }
                }
            }
        }
    }
    return index;
}

int ensure_capacity(size_t required) {
    if (required <= capacity)
        return 1;
    size_t new_capacity = capacity == 0 ? 1 : capacity * 2;
    while (new_capacity < required) {
        new_capacity *= 2;
    }
    Block *temp = realloc(blocks, new_capacity * sizeof(Block));
    if (temp == NULL)
        return 0;
    blocks = temp;
    capacity = new_capacity;
    return 1;
}

int find_block(size_t requested_size) {
    switch (current_strategy) {
        case FIRST_FIT:
            return first_fit(requested_size);
        case BEST_FIT:
            return best_fit(requested_size);
        case WORST_FIT:
            return worst_fit(requested_size);
        default:
            return -1;
    }
}

void allocate_memory(void) {
    size_t requested_size;
    printf("\nEnter requested size: ");
    if (scanf("%zu", &requested_size) != 1) {
        printf("Invalid input!\n");
        return;
    }
    if (requested_size == 0 || requested_size > total_memory) {
        printf("Invalid allocation size!\n");
        return;
    }
    int index = find_block(requested_size);
    if (index == -1) {
        size_t total_free = 0;
        for (size_t i=0;i<number_of_blocks;i++) {
            if (blocks[i].is_free)
                total_free += blocks[i].size;
        }
        printf("Allocation failed: no contiguous free block is large enough.\n");
        if (total_free >= requested_size) {
            printf("External fragmentation detected!\n");
            printf("Total free memory: %zu bytes\n", total_free);
        }
        return;
    }
    size_t remaining = blocks[index].size - requested_size;
    if (remaining > 0) {
        if (!ensure_capacity(number_of_blocks + 1)) {
            printf("Allocation failed: metadata memory unavailable.\n");
            return;
        }
        for(size_t i=number_of_blocks;i>(size_t)index+1;i--) {
            blocks[i] = blocks[i - 1];
        }
        blocks[index + 1].start = blocks[index].start + requested_size;
        blocks[index + 1].size = remaining;
        blocks[index + 1].id = 0;
        blocks[index + 1].is_free = 1;
        number_of_blocks++;
    }
    blocks[index].size = requested_size;
    blocks[index].id = id;
    blocks[index].is_free = 0;
    printf("Allocation successful!\n");
    printf("ID: %d\n", id);
    printf("Start: %zu\n", blocks[index].start);
    printf("Size: %zu bytes\n", requested_size);
    id++;
}

void change_strategy(void) {
    int choice;
    printf("\nSelect allocation strategy:\n");
    printf("1. First Fit\n");
    printf("2. Best Fit\n");
    printf("3. Worst Fit\n");
    printf("Enter choice: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input!\n");
        return;
    }
    if (choice < FIRST_FIT || choice > WORST_FIT) {
        printf("Invalid strategy!\n");
        return;
    }
    current_strategy = (enum Strategy)choice;
    switch (current_strategy) {
        case FIRST_FIT:
            printf("Strategy set to First Fit.\n");
            break;
        case BEST_FIT:
            printf("Strategy set to Best Fit.\n");
            break;
        case WORST_FIT:
            printf("Strategy set to Worst Fit.\n");
            break;
    }
}

void merge_free_blocks(void) {
    size_t i = 0;
    while (i+1 < number_of_blocks) {
        if (blocks[i].is_free && blocks[i + 1].is_free) {
            blocks[i].size += blocks[i + 1].size;
            for (size_t j=i+1;j+1<number_of_blocks;j++) {
                blocks[j] = blocks[j + 1];
            }
            number_of_blocks--;
        }
        else {
            i++;
        }
    }
}

int find_allocation(int allocation_id) {
    for (size_t i = 0; i < number_of_blocks; i++) {
        if (blocks[i].is_free == 0 && blocks[i].id == allocation_id) {
            return (int)i;
        }
    }
    return -1;
}

void release_memory(void) {
    int allocation_id;
    printf("\nEnter allocation ID to release: ");
    if (scanf("%d", &allocation_id) != 1) {
        printf("Invalid input!\n");
        return;
    }
    int index = find_allocation(allocation_id);
    if (index == -1) {
        printf("Invalid allocation ID!\n");
        return;
    }
    blocks[index].is_free = 1;
    blocks[index].id = 0;
    merge_free_blocks();
    printf("Allocation released successfully!\n");
}

void memory_statistics(void) {
    size_t allocated = 0;
    size_t free_memory = 0;
    size_t allocated_blocks = 0;
    size_t free_blocks = 0;
    size_t largest_free = 0;
    for (size_t i=0;i<number_of_blocks;i++) {
        if (blocks[i].is_free == 0) {
            allocated += blocks[i].size;
            allocated_blocks++;
        }
        else {
            free_memory += blocks[i].size;
            free_blocks++;
            if (blocks[i].size > largest_free) {
                largest_free = blocks[i].size;
            }
        }
    }
    printf("\nMEMORY STATISTICS\n");
    printf("Total memory: %zu bytes\n", total_memory);
    printf("Allocated memory: %zu bytes\n", allocated);
    printf("Free memory: %zu bytes\n", free_memory);
    printf("Allocated blocks: %zu\n", allocated_blocks);
    printf("Free blocks: %zu\n", free_blocks);
    printf("Largest free block: %zu bytes\n", largest_free);
}

void resize_memory(void) {
    int allocation_id;
    size_t new_size;
    printf("\nEnter allocation ID: ");
    if (scanf("%d", &allocation_id) != 1) {
        printf("Invalid input!\n");
        return;
    }
    int index = find_allocation(allocation_id);
    if (index == -1) {
        printf("Invalid allocation ID!\n");
        return;
    }
    printf("Enter new size: ");
    if (scanf("%zu", &new_size) != 1) {
        printf("Invalid input!\n");
        return;
    }
    if (new_size == 0 || new_size > total_memory) {
        printf("Invalid resize size!\n");
        return;
    }
    size_t old_size = blocks[index].size;
    size_t old_start = blocks[index].start;
    if (new_size == old_size) {
        printf("Allocation size is already the same!\n");
        return;
    }
    if (new_size < old_size) {
        size_t remaining = old_size - new_size;
        if (!ensure_capacity(number_of_blocks + 1)) {
            printf("Resize failed: metadata memory unavailable.\n");
            return;
        }
        for (size_t i=number_of_blocks;i>(size_t)index+1;i--) {
            blocks[i] = blocks[i - 1];
        }
        blocks[index].size = new_size;
        blocks[index + 1].start = old_start + new_size;
        blocks[index + 1].size = remaining;
        blocks[index + 1].id = 0;
        blocks[index + 1].is_free = 1;
        number_of_blocks++;
        merge_free_blocks();
        printf("Allocation shrunk successfully!\n");
        return;
    }
    size_t extra = new_size - old_size;
    if ((size_t)index + 1 < number_of_blocks && blocks[index + 1].is_free == 1 && blocks[index + 1].size >= extra) {
        blocks[index].size = new_size;
        blocks[index + 1].start += extra;
        blocks[index + 1].size -= extra;
        if (blocks[index + 1].size == 0) {
            for (size_t i = index + 1;
                 i + 1 < number_of_blocks; i++) {
                blocks[i] = blocks[i + 1];
            }
            number_of_blocks--;
        }
        printf("Allocation expanded in place successfully!\n");
        return;
    }
    int new_index = find_block(new_size);
    if (new_index == -1) {
        printf("Resize failed: insufficient contiguous memory.\n");
        return;
    }
    size_t new_start = blocks[new_index].start;
    if (!ensure_capacity(number_of_blocks + 1)) {
        printf("Resize failed: metadata memory unavailable.\n");
        return;
    }
    size_t remaining = blocks[new_index].size - new_size;
    blocks[new_index].size = new_size;
    blocks[new_index].id = allocation_id;
    blocks[new_index].is_free = 0;
    if (remaining > 0) {
        for (size_t i=number_of_blocks;i>(size_t)new_index+1;i--) {
            blocks[i] = blocks[i - 1];
        }
        blocks[new_index + 1].start = new_start + new_size;
        blocks[new_index + 1].size = remaining;
        blocks[new_index + 1].id = 0;
        blocks[new_index + 1].is_free = 1;
        number_of_blocks++;
    }
    index = -1;
    for (size_t i = 0; i < number_of_blocks; i++) {
        if (blocks[i].start == old_start && blocks[i].id == allocation_id && blocks[i].is_free == 0) {
            index = (int)i;
            break;
        }
    }
    if (index == -1) {
        printf("Resize failed: metadata inconsistency.\n");
        return;
    }
    if (new_start < old_start) {
        for (size_t i=0;i<old_size;i++) {
            memory[new_start + i] = memory[old_start + i];
        }
    }
    else {
        for (size_t i=old_size;i>0;i--) {
            memory[new_start + i - 1] = memory[old_start + i - 1];
        }
    }
    blocks[index].is_free = 1;
    blocks[index].id = 0;
    merge_free_blocks();
    printf("Allocation moved and resized successfully!\n");
}

int main() {
    size_t pool_size;
    printf("Enter size : ");
    if(scanf("%zu", &pool_size) != 1) {
        printf("Invalid input!\n");
        return 1;
    }
    if(pool_size < 1 || pool_size > 1000000) {
        printf("Invlaid Input\n");
        return 1;
    }
    if(initialize_memory(pool_size) == 0) {
        printf("Memory initialization failed!\n");
        return 1;
    }
    int choice;
    printf("\nSelect allocation strategy:\n");
    printf("1. First Fit\n");
    printf("2. Best Fit\n");
    printf("3. Worst Fit\n");
    printf("Enter choice: ");
    if(scanf("%d", &choice) != 1 || choice < FIRST_FIT || choice > WORST_FIT) {
        printf("Invalid strategy!\n");
        free(memory);
        free(blocks);
        return 1;
    }
    current_strategy = (enum Strategy)choice;
    while (1) {
        printf("\nSMART MEMORY MANAGER\n");
        printf("1. Allocate memory\n");
        printf("2. Release memory\n");
        printf("3. Resize an allocation\n");
        printf("4. Display memory map\n");
        printf("5. Display memory statistics\n");
        printf("6. Change allocation strategy\n");
        printf("7. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            break;
        }
        switch (choice) {
            case 1:
                allocate_memory();
                break;
            case 2:
                release_memory();
                break;
            case 3:
                resize_memory();
                break;
            case 4:
                display_memory_map();
                break;
            case 5:
                memory_statistics();
                break;
            case 6:
                change_strategy();
                break;
            case 7:
                printf("\nExiting Smart Memory Manager...\n");
                free(memory);
                free(blocks);
                return 0;
            default:
                printf("Invalid menu choice!\n");
        }
    }
    free(memory);
    free(blocks);
    return 0;
}


