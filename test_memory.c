#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main()
{
    int *ptr;

    ptr = (int *)malloc(sizeof(int));

    assert(ptr != NULL);

    *ptr = 100;

    assert(*ptr == 100);

    free(ptr);

    printf("Memory Test Passed\n");

    return 0;
}