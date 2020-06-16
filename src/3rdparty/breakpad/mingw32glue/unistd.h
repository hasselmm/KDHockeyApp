#pragma once

#include_next <unistd.h>

#include <sys/stat.h>

#ifndef __WORDSIZE
#ifdef __x86_64__
#define __WORDSIZE 64
#else
#define __WORDSIZE 32
#endif
#endif

#ifndef __BIG_ENDIAN
# define __BIG_ENDIAN 4321
#endif
#ifndef __LITTLE_ENDIAN
# define __LITTLE_ENDIAN 1234
#endif

#ifndef __BYTE_ORDER
# define __BYTE_ORDER __LITTLE_ENDIAN
#endif

#if __cplusplus
extern "C" {
#endif
int getpagesize(void);
ssize_t pread(int fd, void *buf, size_t count, off_t offset);
#if __cplusplus
}
#endif

#define kernel_stat stat
#define kernel_stat64 stat64
#define sys_close close
#define sys_open open
#define sys_fstat fstat
#define sys_fstat64 fstat64
