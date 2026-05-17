#ifndef BLOCK_H

#define BLOCK_H

#include <stddef.h>

typedef struct Node {
    size_t size;
    int free;

    Node *next, *prev;
} Node;

#endif