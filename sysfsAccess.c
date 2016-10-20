#define _GNU_SOURCE
#include <limits.h>
#include <glob.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "sysfsAccess.h"

int sysfsOpenFmt(const char* patternformat, ...)
{
    int fd;
    va_list ap;
    
    va_start(ap, patternformat);
    fd = sysfsOpenVar(patternformat, ap);
    va_end(ap);
    return fd;
}

int sysfsOpenVar(const char* patternformat, va_list ap)
{
    char *pattern;
    int fd;
    
    vasprintf(&pattern, patternformat, ap);
    fd = sysfsOpen(pattern);
    free(pattern);
    return fd;
}

int sysfsOpen(const char* pattern)
{
    glob_t globresults;
    int fd;

    if (glob(pattern, GLOB_BRACE|GLOB_TILDE, NULL, &globresults) != 0)
    {
        /*errno = ENOENT;*/
        return -1;
    }
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
