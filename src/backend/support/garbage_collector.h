#ifndef COMPILER_GARBAGE_COLLECTOR_H
#define COMPILER_GARBAGE_COLLECTOR_H

#include <stddef.h>

void * Malloc(size_t size);

void * Calloc(size_t count, size_t size);

void * Realloc(void * ptr, size_t size);

void Free_All();

#endif //COMPILER_GARBAGE_COLECTOR_H
