#pragma once

#include_next <string.h>

#if __cplusplus
extern "C" {
#endif

void *memrchr (const void *s, int c, size_t n);

#if __cplusplus
}
#endif
