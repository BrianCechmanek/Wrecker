/*  Wrecker
 *   sys.c
 *   Source file for Wrapper/System functions.
 *   Example: Error-Memory-Alloc (emalloc)
 *            
 */
#include <stdio.h>
#include <stdlib.h>
#include "dbg.h"

// Throws an Error and crashed if memory allocation fails, saving us from testing
// it every damn time.
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
