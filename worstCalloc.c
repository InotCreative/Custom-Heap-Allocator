#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>
#include <string.h>

void *callocWorst(size_t numberOfElements, size_t elementSize) {
    size_t elementSizeBytes = numberOfElements * elementSize;
    /*
    Request the number of bytes that holds the number of elements desired by the user
    and the size of data type of the elements.

    sizeof(int)    -> 4
    sizeof(char)   -> 1
    sizeof(float)  -> 4
    sizeof(double) -> 8
    sizeof(void)   -> 1
    */

    void *heapEnd = (void *)sbrk(0);
    /*
    The syscall sbrk(0) returns the current end of the heap, known as the 
    "program break." This is the starting point for any new memory allocation.
    */

    void *firstFreeHeapChunk = (void *)sbrk(elementSizeBytes);
    /*
    The syscall sbrk(size) increments the program break by the specified size,
    expanding the heap by this amount. The function returns the starting address
    of the newly allocated memory.
    */

   if (firstFreeHeapChunk == ((void *)-1)) {
        return NULL;

        /*
        An unsucessful execuation of sbrk returns -1, and beacuse a void pointer is
        being used to store the memory address, the code needs to check if the run
        was unsucessful, and if thats the case return NULL.
        */
   } else {
        assert(heapEnd == firstFreeHeapChunk);
        memset(firstFreeHeapChunk, 0, elementSizeBytes);

        return firstFreeHeapChunk;

        /*
        If the memory allocation was successful, return the starting address of
        the newly allocated block, which is the old program break.
        */
    }
}

int main(int argc, char const *argv[]) {
    char *alphabet = callocWorst(27, sizeof(char));

    for (int index = 0; index < 26; index++) {
        printf("%d\n", alphabet[index]);
    }

    alphabet[26] = '\0';
    printf("{%s}\n\n", alphabet);

    for (int index = 0; index < 26; index++) {
        alphabet[index] = index + 'A';
        printf("%c\n", alphabet[index]);
    }

    alphabet[26] = '\0';

    printf("{%s}\n", alphabet);

    return 0;
}
