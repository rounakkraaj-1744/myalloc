#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allocator.h>

#define NUM_PTRS 1000

int main() {
    allocator_init();

    void *ptrs[NUM_PTRS] = {0};

    srand(time(NULL));

    for (int i = 0; i < 10000; i++) {
        int idx = rand() % NUM_PTRS;
        if (ptrs[idx]) {
            myfree(ptrs[idx]);
            ptrs[idx] = NULL;
        }

        else {
            size_t size = (rand() % 256) + 1;
            ptrs[idx] = myalloc(size);
        }
    }

    printf("Stress test completed\n");
}