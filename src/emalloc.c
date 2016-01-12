#include "emalloc.h"

void *emalloc(size_t size)
{
    void *p;
    p = malloc(size);
    if (p == NULL) {
        fprintf(stderr, "Memory allocation error. FILE: %s LINE: %d\n", __FILE__, __LINE__);
        exit(1);
    }
    return p;
}

int main(void)
{
    char *test = emalloc(10);
    free(test);
    printf("Woot!\n");
    return 0;
}
