#ifndef sysfsAccess_h
#define sysfsAccess_h

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

int sysfsOpen(const char*  path);
int sysfsReadStr(int fd, char* buffer, int buffersize);
unsigned long sysfsReadULong(int fd);
long sysfsReadLong(int fd);
int sysfsWrite(int fd, const char* fmt, long value);

#ifdef __cplusplus
}
#endif
#endif
