#ifndef NODE_H

#define NODE_H

#include <stddef.h>

typedef struct Node {
    size_t size;
    int free;

    struct Node *next, *prev;
} Node;

typedef struct Footer {
    size_t size;
    int free;
} Footer;

#endif