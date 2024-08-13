#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>

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

int main(int argc, char const *argv[])
{
    char *alphabhet = (char *)malloc(27 * sizeof(char));

    /*
    The function will allocate enough space for 27 characters, and this array
    will decay to the memeory address of the first character.
    */

    for (int index = 0; index < 26; index++) {
        alphabhet[index] = 'A' + index;

        printf("%c\n", alphabhet[index]);
    }

    alphabhet[26] = '\0';

    printf("%s\n", alphabhet);

    return 0;
}
