#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

int __vsnprintf(char *restrict, size_t, const char *restrict, va_list);

int __vasprintf(char **s, const char *fmt, va_list ap)
{
	va_list ap2;
	va_copy(ap2, ap);
	int l = __vsnprintf(0, 0, fmt, ap2);
	va_end(ap2);

	if (l<0 || !(*s=malloc(l+1U))) return -1;
	return __vsnprintf(*s, l+1U, fmt, ap);
}

int ___asprintf(char **s, const char *fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = __vasprintf(s, fmt, ap);
	va_end(ap);
	return ret;
}

#ifdef _LIBC
#include "libioP.h"
ldbl_weak_alias   (__vasprintf, vasprintf)
ldbl_hidden_def   (___asprintf, __asprintf)
ldbl_strong_alias (___asprintf, __asprintf)
ldbl_weak_alias   (___asprintf, asprintf)
#endif
