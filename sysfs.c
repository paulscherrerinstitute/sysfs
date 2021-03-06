#include <glob.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "sysfs.h"

#ifndef GLOB_BRACE
#define GLOB_BRACE 0
#endif

int sysfsOpen(const char* path)
{
    glob_t globresults;
    int fd;

    int status = glob(path, GLOB_BRACE, NULL, &globresults);
     
    if (status == GLOB_NOMATCH)
    {
        errno = ENOENT;
        return -1;
    }
    if (status != 0)
        return -1;
    fd = open(globresults.gl_pathv[0], O_RDWR);
    if (fd < 0)
        fd = open(globresults.gl_pathv[0], O_RDONLY);
    globfree(&globresults);
    return fd;
}

int sysfsReadStr(int fd, char* buffer, int buffersize)
{
    if (lseek(fd, 0, SEEK_SET) < 0)
        return -1;
    buffersize = read(fd, buffer, buffersize-1);
    if (buffersize < 0)
        return -1;
    return buffersize;
}

unsigned long sysfsReadULong(int fd)
{
    char buffer[32] = {0};

    if (sysfsReadStr(fd, buffer, sizeof(buffer)) < 0)
        return -1;
    return strtoul(buffer, NULL, 0);
}

long sysfsReadLong(int fd)
{
    char buffer[32] = {0};

    if (sysfsReadStr(fd, buffer, sizeof(buffer)) < 0)
        return -1;
    return strtol(buffer, NULL, 0);
}

int sysfsWrite(int fd, const char* fmt, long value)
{
    int n;
    char buffer[16];
    n = snprintf(buffer, sizeof(buffer), fmt, value);
    if (write(fd, buffer, n) < 0)
            return -1;
    return n;
}
