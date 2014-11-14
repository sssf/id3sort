#include <stdlib.h>
#include <stdio.h>


typedef struct Alloc_info {
    void   *address;
    size_t  size;
    char   *file;
    char   *function;
    int     line;
    struct Alloc_info *next;
} Alloc_info;

Alloc_info allocations;    // used to track allocations
Alloc_info deallocations;  // used to detect double frees

// create something like void track_alloc();


void* remember_alloc(size_t size, const char* file, const char* function, int line) {
    printf("alloc: %zd bytes   file: %s function: %s line: %d\n", size, file, function, line);
    return malloc(size);
}

void  remember_delete(void* ptr, const char* file, const char* function, int line) {
    printf("delete: %p file: %s function: %s line: %d\n", ptr, file, function, line);
    free(ptr);
}

