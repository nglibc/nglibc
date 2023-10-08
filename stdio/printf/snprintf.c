#define _GNU_SOURCE 1
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <stdarg.h>

FILE * __finit   (FILE *, cookie_io_functions_t *, void *, char *, size_t);
int    __vfprintf(FILE *, const char *, va_list);

int __vsnprintf(char *restrict s, size_t n, const char *restrict fmt, va_list ap)
{
	static cookie_io_functions_t iof;
	if (n <= INT_MAX) {
		FILE file = {}, *f = __finit(&file, &iof, 0, s, n ? n-1:0);
		int ret = __vfprintf(f, fmt, ap);
		if ((unsigned)ret < n) s[ret] = 0; // null terminate only if ret >= 0 & n > 0
		return ret;
	} else return (void)(errno = EOVERFLOW), -1;
}

int __snprintf(char *restrict s, size_t n, const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = __vsnprintf(s, n, fmt, ap);
	va_end(ap);
	return ret;
}

#ifdef _LIBC
#include "libioP.h"
ldbl_weak_alias (__vsnprintf, vsnprintf)
libc_hidden_def (__snprintf)
ldbl_weak_alias (__snprintf,  snprintf)
#endif
