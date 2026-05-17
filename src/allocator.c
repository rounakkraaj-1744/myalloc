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
static Footer* get_footer(Node *node);
static void write_footer(Node *node);
static void coalesce(Node *node);

static Footer* get_footer(Node *node) {
    return (Footer*)((char*)node + sizeof(Node) + node->size);
}

static void write_footer(Node *node) {
    Footer *footer = get_footer(node);
    footer->size = node->size;
    footer->free = node->free;
}

void allocator_init() {
    heap_start = (Node*) memory_pool;
    heap_start->size = POOL_SIZE - sizeof(Node) - sizeof(Footer);
    heap_start->free = 1;
    heap_start->next = NULL;
    heap_start->prev = NULL;

    write_footer(heap_start);
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
    if (node->size <= size + sizeof(Node) + sizeof(Footer) + MIN_SPLIT_SIZE)
        return;

    Node *newNode = (Node*)((char*)node + sizeof(Node) + size + sizeof(Footer));

    newNode->size = node->size - size - sizeof(Node) - sizeof(Footer);
    newNode->free = 1;
    newNode->next = NULL;
    newNode->prev = NULL;

    node->size = size;
    write_footer(node);
    write_footer(newNode);

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
    write_footer(node);

    return (void*)(node + 1);
}

static Node* get_next_node(Node *node) {
    char *next = (char*)node + sizeof(Node) + node->size + sizeof(Footer);

    if (next >= memory_pool + POOL_SIZE)
        return NULL;

    return (Node*) next;
}

static Node* get_prev_node(Node *node) {
    if (node == heap_start)
        return NULL;

    Footer *footer = (Footer*)((char*)node - sizeof(Footer));
    size_t prev_size = footer->size;
    Node *prev = (Node*)((char*)node - sizeof(Footer) - sizeof(Node) - prev_size);

    if ((char*)prev < memory_pool)
        return NULL;

    return prev;
}

static void coalesce(Node *node) {
    Node *next = get_next_node(node);

    if (next && next->free) {
        delete_node(next);
        node->size += sizeof(Node) + sizeof(Footer) + next->size;
    }

    Node *prev = get_prev_node(node);

    if (prev && prev->free) {
        delete_node(prev);
        prev->size += sizeof(Node) + sizeof(Footer) + node->size;
        node = prev;
    }

    write_footer(node);
    insert_node(node);
}

void myfree(void *ptr) {
    if (!ptr)
        return;

    Node *node = ((Node*)ptr) - 1;
    node->free = 1;
    write_footer(node);

    coalesce(node);
}