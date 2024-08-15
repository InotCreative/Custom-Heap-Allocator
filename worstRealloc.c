#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>
#include <string.h>

void *malloc(size_t size) {
    void *p = sbrk(0);
    /*
    The syscall sbrk(0) returns the current end of the heap, known as the 
    "program break." This is the starting point for any new memory allocation.
    */

    void *request = sbrk(size);
    /*
    The syscall sbrk(size) increments the program break by the specified size,
    expanding the heap by this amount. The function returns the starting address
    of the newly allocated memory.
    */

    if (request == (void*)-1) {
        return NULL;
        /*
        An unsucessful execuation of sbrk returns -1, and beacuse a void pointer is
        being used to store the memory address, the code needs to check if the run
        was unsucessful, and if thats the case return NULL.
        */
    } else {
        assert(p == request);
        return p;
        /*
        If the memory allocation was successful, return the starting address of
        the newly allocated block, which is the old program break.
        */
    }
}

void *realloc(void *memory, size_t size) {
    if (memory == NULL) return NULL;
    /*
    If there is is no memeory allocated then allocated space of the heap for the
    desired amount of bytes.
    */

    void *newPointer = malloc(size);
    /*
    Create a new new pointer to the location of memeory of the larger bytes of data.
    When realloc is called, the goal is to create a new space of memeory that is either
    greater or smaller than the original space, therefore, a new space needs to be found
    on the heap that mataches the new bytes requested at runtime.
    */

    if (newPointer == NULL) {
        return NULL;

        /*
        If that space can't be allocated then return NULL.
        */
    }

    memcpy(newPointer, memory, size);

    /*
    To maintain a small amount of memory safety, copy the contents form the original request
    to the memory to the new space allocated on the heap. 
    */

    return newPointer;
    /*
    Return the new void pointer to the memory.
    */
}

int main(int argc, char const *argv[]) {
    int bytes = 26;
    char *alphabhet = (char *)malloc(bytes * sizeof(char));

    /*
    The function will allocate enough space for 27 characters, and this array
    will decay to the memeory address of the first character.
    */

    for (int index = 0; index < 26; index++) {
        alphabhet[index] = 'A' + index;

        printf("%c\n", alphabhet[index]);
    }

    printf("%s", alphabhet);

    alphabhet = realloc(alphabhet, bytes + 27);

    for (int index = 26, count = 0; index < bytes + 27; index++, count++) {
        alphabhet[index] = 'a' + count;

        printf("%c\n", alphabhet[index]);
    }

    alphabhet[bytes + 26] = '\0'; 

    printf("%s\n", alphabhet);

    return 0;
}
