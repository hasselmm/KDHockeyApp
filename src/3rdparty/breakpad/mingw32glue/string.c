#include <stddef.h>

#include "string.h"

void *memrchr(const void *s, int c, size_t n)
{
    for (const char *p = s + n; --p >= (const char *) s; ) {
        if (*p == c)
            return (void *) p; // WTF
    }

    return NULL;
}
