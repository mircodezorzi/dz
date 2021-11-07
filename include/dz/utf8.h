#ifndef __DZ_UTF8_H__
#define __DZ_UTF8_H__

#include <stdarg.h>
#include <stddef.h>
#include <sys/types.h>

int u8_toucs(u_int32_t *dest, int sz, char *src, int srcsz);
int u8_vprintf(char *fmt, va_list ap);
int u8_printf(char *fmt, ...);

#endif /* __DZ_UTF8_H__ */
