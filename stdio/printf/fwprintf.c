#include <wchar.h>
#include <stdarg.h>

int __vfwprintf(FILE *restrict, const wchar_t *restrict, va_list);

int __fwprintf(FILE *restrict f, const wchar_t *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = __vfwprintf(f, fmt, ap);
	va_end(ap);
	return ret;
}


#ifdef _LIBC
#include "libioP.h"
ldbl_weak_alias (__fwprintf,  fwprintf)
#endif
