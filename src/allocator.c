#include <stdio.h>
#include <string.h>
#include <allocator.h>
#include <node.h>
#include <freelist.h>
#include <utils.h>
#include <config.h>

char memory_pool[POOL_SIZE];

Node *heap_start = NULL;

static Node* find_fit(size_t size);
static void split_node(Node *node, size_t size);
static Node* get_next_node(Node *node);
static Node* get_prev_node(Node *node);
static void coalesce(Node *node);

void allocator_init() {
    heap_start = (Node*) memory_pool;
    heap_start->size = POOL_SIZE - sizeof(Node);
    heap_start->free = 1;
    heap_start->next = NULL;
    heap_start->prev = NULL;

    insert_node(heap_start);
}

static Node* find_fit(size_t size) {
    int class = get_size_class(size);

    for (int i = class; i < NUM_CLASSES; i++) {
        Node *curr = free_lists[i];

        while (curr) {
            if (curr->free && curr->size >= size)
                return curr;
            curr = curr->next;
        }
    }

    return NULL;
}

static void split_node(Node *node, size_t size) {
    if (node->size <= size + sizeof(Node) + MIN_SPLIT_SIZE)
        return;
    
    Node *newNode = (Node*)((char*)node + sizeof(Node) + size);

    newNode->size = node->size - size - sizeof(Node);
    newNode->free = 1;
    newNode->next = NULL;
    newNode->prev = NULL;

    node->size = size;

    insert_node(newNode);
}

void* myalloc(size_t size) {
    if (size == 0)
        return NULL;

    size = align_size(size);

    Node *node = find_fit(size);
    if (!node)
        return NULL;

    delete_node(node);
    split_node(node, size);
    node->free = 0;

    return (void*)(node + 1);
}

static Node* get_next_node(Node *node) {

    char *next = (char*)node + sizeof(Node) + node->size;

    if (next >= memory_pool + POOL_SIZE)
        return NULL;

    return (Node*) next;
}

static Node* get_prev_node(Node *node) {

    Node *curr = heap_start;
    Node *prev = NULL;

    while (curr && curr != node) {
        prev = curr;
        curr = get_next_node(curr);
    }

    return prev;
}

static void coalesce(Node *node) {
    Node *next = get_next_node(node);

    if (next && next->free) {
        delete_node(next);
        node->size += sizeof(Node) + next->size;
    }

    Node *prev = get_prev_node(node);

    if (prev && prev->free) {
        delete_node(prev);
        prev->size += sizeof(Node) + node->size;
        node = prev;
    }

    insert_node(node);
}

void myfree(void *ptr) {

    if (!ptr)
        return;

    Node *node = ((Node*)ptr) - 1;

    node->free = 1;

    coalesce(node);
}