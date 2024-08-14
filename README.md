# Custom Heap Allocator in C

This project implements a custom heap allocator in C as part of an experimental exploration into dynamic memory management techniques. The allocator is designed to handle memory allocation and deallocation with the following key features:

## Features

- **Custom-Sized Memory Blocks**: The allocator supports varying block sizes, enabling efficient management of memory fragmentation and utilization.
  
- **Memory Pool Implementation**: A memory pool is used to pre-allocate memory, allowing for more efficient resource management by minimizing frequent system calls for memory.

- **Tracking Allocated and Free Blocks**: A system is in place to keep track of which memory blocks are allocated and which are free, facilitating optimized memory reuse and management.

- **Thread Safety**: The allocator includes locking mechanisms to ensure correct operation in multithreaded environments, preventing race conditions and ensuring data integrity.

- **Performance Considerations**: The allocator is optimized to balance low latency and high throughput, providing a performance-oriented memory management solution.

## Purpose

The primary objective of this project was to gain a practical understanding of memory management techniques at a low level, enhance C programming skills, and explore the intricacies of system-level memory operations.

## Example Code

```c
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
        An unsuccessful execution of sbrk returns -1. Since a void pointer is
        used to store the memory address, the code must check for an unsuccessful
        run, returning NULL if that is the case.
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
    char *alphabet = (char *)malloc(27 * sizeof(char));

    /*
    The function allocates enough space for 27 characters, and this array
    will decay to the memory address of the first character.
    */

    for (int index = 0; index < 26; index++) {
        alphabet[index] = 'A' + index;

        printf("%c\n", alphabet[index]);
    }

    alphabet[26] = '\0';

    printf("%s\n", alphabet);

    return 0;
}

```
Worst Code Example
The code provided above is the worst-case scenario for a custom heap allocator. It relies heavily on the sbrk system call, which is considered unsafe and outdated in modern systems programming. Here’s why:

No Memory Deallocation: The malloc function does not include a free counterpart, meaning that allocated memory cannot be released, leading to memory leaks.

Thread Safety: The function is not thread-safe, which can lead to undefined behavior in multithreaded programs.

No Error Handling: The code lacks robust error handling, making it prone to crashes or undefined behavior if something goes wrong during memory allocation.

Fragmentation Issues: The use of sbrk directly manipulates the program break, potentially leading to severe memory fragmentation issues.

This code example demonstrates the need for a more sophisticated approach to memory management, which the custom heap allocator aims to address.
