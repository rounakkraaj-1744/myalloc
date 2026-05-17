#include <assert.h>
#include <allocator.h>

int main() {
    allocator_init();

    int *x = (int*) myalloc(sizeof(int));

    assert(x != NULL);

    *x = 100;

    assert(*x == 100);

    myfree(x);
}