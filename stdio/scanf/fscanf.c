#ifdef _LIBC
#include <features.h>
#undef  __GLIBC_USE_DEPRECATED_SCANF
#define __GLIBC_USE_DEPRECATED_SCANF 1
#endif
#include <stdio.h>
#include <stdarg.h>

int __vfscanf(FILE *restrict, const char *restrict, va_list);

int __fscanf(FILE *restrict f, const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = __vfscanf(f, fmt, ap);
	va_end(ap);
	return ret;
}


#ifdef _LIBC
#include "libioP.h"
ldbl_strong_alias (__fscanf, __isoc99_fscanf)
ldbl_strong_alias (__fscanf, fscanf)
#endif
