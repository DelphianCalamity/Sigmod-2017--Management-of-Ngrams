#include <stdio.h>
#include <stdlib.h>

void getError(int code) {
    switch (code) {
        case 1:
            perror("Error in memory allocation");
            break;

        case 2:
            perror("Error in memory reallocation");
            break;

        case 3:
            perror("Error in file opening");
            break;

        case 4:
            perror("Error in popping stack");
            break;
    }
    return;
}

void* safemalloc(size_t size) {
    void* pointer = malloc(size);
    if (pointer == NULL) {
        getError(1);
        exit(1);
    }
    return pointer;
}

void* safecalloc(size_t num, size_t size) {
    void* pointer = calloc(num, size);
    if (pointer == NULL) {
        getError(1);
        exit(1);
    }
    return pointer;
}

void* saferealloc(void* ptr, size_t size) {
    void* pointer = realloc(ptr, size);
    if (pointer == NULL) {
        getError(2);
        exit(1);
    }
    return pointer;
}