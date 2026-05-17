#ifndef FREELIST_H

#define FREELIST_H

#include <node.h>

extern Node *free_list[];

void insert_node(Node *node);
void delete_node(Node *node);
int get_size_class(size_t size);

#endif