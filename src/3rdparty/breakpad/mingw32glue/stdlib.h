#pragma once

#include_next "stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

char *realpath(const char *path, char *resolved);

#ifdef __cplusplus
}
#endif
