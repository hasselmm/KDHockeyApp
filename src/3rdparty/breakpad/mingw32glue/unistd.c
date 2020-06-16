#include <sys/types.h>
#include <unistd.h>

ssize_t pread(int fd, void *buf, size_t count, off_t offset)
{
    lseek(fd, offset, SEEK_SET);
    return read(fd, buf, count);
}

