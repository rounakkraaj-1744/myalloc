#include <stdio.h>
#include <allocator.h>
#include <utils.h>

int main() {
    allocator_init();

    int *a = (int*) myalloc(sizeof(int));
    int *b = (int*) myalloc(sizeof(int) * 10);
    char *c = (char*) myalloc(100);

    *a = 42;

    printf("a = %d\n", *a);

    print_heap_layout();

    myfree(b);

    print_heap_layout();

    myfree(a);
    myfree(c);

    print_heap_layout();
}