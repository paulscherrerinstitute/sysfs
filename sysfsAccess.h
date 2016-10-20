#ifndef sysfsAccess_h
#define sysfsAccess_h

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

int sysfsOpenFmt(const char* patternformat, ...) __attribute__ ((__format__ (__printf__, 1, 2)));
int sysfsOpenVar(const char* patternformat, va_list ap) __attribute__ ((__format__ (__printf__, 1, 0)));
int sysfsOpen(const char* pattern) __attribute__ (( __nonnull__ (1)));
int sysfsReadStr(int fd, char* buffer, int buffersize) __attribute__ (( __nonnull__ (2)));
unsigned long sysfsReadULong(int fd);
long sysfsReadLong(int fd);
int sysfsWrite(int fd, const char* fmt, long value)  __attribute__ ((__format__ (__printf__, 2, 0)));

#ifdef __cplusplus
}
#endif
#endif