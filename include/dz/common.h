#ifndef __DZ_COMMON_H__
#define __DZ_COMMON_H__

#if defined(__STDC__) && !defined(__STDC_VERSION__)
typedef int bool;
#define true 1
#define false 0

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#elif __STDC_VERSION__ >= 199901L

#include <stdbool.h>

#define max(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a > _b ? _a : _b;                                                         \
  })

#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

#endif /* __ANSI__ */

#endif /* __DZ_COMMON_H__ */
