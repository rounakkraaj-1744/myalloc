#ifndef ALLOCATOR_H

#define ALLOCATOR_H

#include <stddef.h>

void allocator_init();

void *myalloc(size_t size);
void myfree(void *ptr);

#endif