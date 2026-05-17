#include <stdio.h>
#include <utils.h>
#include <node.h>
#include <config.h>

extern char memory_pool[];
extern Node *heap_start;

size_t align_size (size_t size) {
    return (size + (ALIGNMENT -1 )) & ~(ALIGNMENT - 1);
}

void print_heap_layout() {
    Node *curr = heap_start;

    printf("\n===== HEAP LAYOUT =====\n");

    while ((char*)curr < memory_pool + POOL_SIZE) {
        const char *status = curr->free ? "FREE" : "USED";
        printf("Status: %s | Size: %zu bytes\n", status, curr->size);

        curr = (Node*)((char*)curr + sizeof(Node) + curr->size);

        if ((char*)curr >= memory_pool + POOL_SIZE)
            break;
    }

    printf("========================\n");
}