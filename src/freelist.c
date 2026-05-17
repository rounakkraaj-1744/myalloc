#include <freelist.h>
#include <config.h>

Node *free_lists[NUM_CLASSES] = {0};

int get_size_class(size_t size) {
    if (size <= 16)
        return 0;
    if (size <= 32)
        return 1;
    if (size <= 64)
        return 2;
    if (size <= 128)
        return 3;
    if (size <= 256)
        return 4;
    if (size <= 512)
        return 5;
    if (size <= 1024)
        return 6;
    return 7;
}

void insert_node(Node *node) {
    int class = get_size_class(node->size);

    node->next = free_lists[class];
    node->prev = NULL;

    if (free_lists[class])
        free_lists[class]->prev = node;

    free_lists[class] = node;
}

void delete_node(Node *node) {
    int class = get_size_class(node->size);

    if (node->prev)
        node->prev->next = node->next;
    else
        free_lists[class] = node->next;

    if (node->next){
        node->next->prev = node->prev;
    }

    node->next = NULL;
    node->prev = NULL;
}