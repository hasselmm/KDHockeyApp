#include "stdlib.h"

#include <limits.h>

char *_fullpath(char *absPath, const char *relPath, size_t maxLength);

char *realpath(const char *path, char *resolved)
{
    return _fullpath(resolved, path, PATH_MAX);
}

