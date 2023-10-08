#ifdef _LIBC
#include <features.h>
#undef  __GLIBC_USE_DEPRECATED_SCANF
#define __GLIBC_USE_DEPRECATED_SCANF 1
#endif
#include <wchar.h>
#include <stdarg.h>

int __vfwscanf(FILE *restrict f, const wchar_t *restrict fmt, va_list);

int __fwscanf(FILE *restrict f, const wchar_t *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = __vfwscanf(f, fmt, ap);
	va_end(ap);
	return ret;
}

#ifdef _LIBC
#include "libioP.h"
ldbl_strong_alias (__fwscanf,  __isoc99_fwscanf)
ldbl_strong_alias (__fwscanf, fwscanf)
#endif
