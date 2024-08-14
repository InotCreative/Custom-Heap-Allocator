#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>

typedef struct blockMeta {
    size_t size;
    struct blockMeta *next;
    int free;
} blockMeta;

/*
This is the definition of a singly linked list node, which stores heap information,
including the size of the memory block and a pointer to the next memory block in the heap.
*/

#define META_SIZE sizeof(blockMeta)

void *globalBase = NULL;

blockMeta *findFreeBlock(blockMeta **last, size_t size) {
    blockMeta *current = globalBase; // Start with the global base of the linked list heap.

    /*
    Traverse the linked list to find a free block that is large enough to satisfy the request.
    The loop continues until a suitable free block is found or the end of the list is reached.
    */

    while (current && !(current->free && current->size >= size)) {
        *last = current;
        current = current->next;

        // Update the last pointer to the current block and move to the next block.
    }

    return current; // Return the first suitable free block found, or NULL if none exists.
}

blockMeta *requestSpace(blockMeta *last, size_t size) {
    blockMeta *block;

    block = (blockMeta *)sbrk(0);
    /*
    The sbrk(0) system call returns the current end of the heap, also known as the "program break."
    This serves as the starting point for any new memory allocation.
    */

    void *request = sbrk(size + META_SIZE);
    /*
    The sbrk(size + META_SIZE) system call increments the program break by the specified size, expanding
    the heap by this amount. It returns the starting address of the newly allocated memory.
    */

    assert((void *)block == (void *)request); 
    /*
    Ensure that the memory allocation was successful by checking that the start of the newly allocated
    memory matches the old program break. If they don't match, the assertion will fail.
    */

    if (request == (void *)-1) {
        return NULL;

        /*
        An unsuccessful execution of sbrk returns -1. Since a void pointer is used to store
        the memory address, the code needs to check if the run was unsuccessful, and in that case,
        return NULL.
        */
    }

    block->size = size;
    block->next = NULL;
    block->free = 0;

    return block;

    /* 
    Return the address of the block of memory that was allocated by sbrk.
    */
}

void *malloc(size_t size) {
    blockMeta *block;

    if (size <= 0) return NULL; 
    /*
    Ensure that the requested size is greater than 0 because it's not possible to allocate
    0 or negative memory.
    */

    if (!globalBase) { // If this is the first allocation (globalBase is NULL)
        block = requestSpace(NULL, size);
        /*
        If globalBase is NULL, request memory. Since this is the first allocation,
        there is no previous block.
        */

        if (!block) return NULL; // If the space can't be allocated, return NULL.

        globalBase = block; // Set the global base to the first allocated memory block.

    } else { // If globalBase is not NULL
        blockMeta *last = globalBase; // Start from the global base.
        block = findFreeBlock(&last, size); 

        /*
        Find the next free block that is large enough to satisfy the memory request.
        */

        if (!block) { // If no suitable free block is found
            block = requestSpace(last, size); // Request new memory.

            if (!block) return NULL; // If memory can't be allocated, return NULL.

        } else block->free = 0; // Mark the block as allocated (not free).

    }

    return block + 1; // Return a pointer to the memory just after the metadata block.
}

blockMeta *getBlockPtr(void *ptr) {
    return (blockMeta *)ptr - 1; 
    /*
    Given a pointer to the allocated memory, return a pointer to the corresponding
    blockMeta structure, which is located just before the memory block.
    */
}

int main(int argc, char const *argv[]) {
    char *alphabet = (char *)malloc(27 * sizeof(char));

    /*
    The function allocates enough space for 27 characters, including the null terminator.
    */

    for (int index = 0; index < 26; index++) {
        alphabet[index] = 'A' + index;

        printf("%c\n", alphabet[index]);

    }

    alphabet[26] = '\0';

    printf("%s\n", alphabet);

    return 0;
}
