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
        default:break;
    }
}

void* safemalloc(int size) {
    void* pointer = malloc((size_t)size);
    if (pointer == NULL) {
        getError(1);
        exit(1);
    }
    return pointer;
}

void* safecalloc(int num, int size) {
    void* pointer = calloc((size_t)num, (size_t)size);
    if (pointer == NULL) {
        getError(1);
        exit(1);
    }
    return pointer;
}

void* saferealloc(void* ptr, int size) {
    void* pointer = realloc(ptr,(size_t)size);
    if (pointer == NULL) {
        getError(2);
        exit(1);
    }
    return pointer;
}