#include <stdio.h>
#include <limits.h>
#include <stdarg.h>

int __vsnprintf(char *restrict, size_t, const char *restrict, va_list);

int __vsprintf(char *restrict s, const char *restrict fmt, va_list ap)
{
	return __vsnprintf(s, INT_MAX, fmt, ap);
}

int __sprintf(char *restrict s, const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = __vsprintf(s, fmt, ap);
	va_end(ap);
	return ret;
}


#ifdef _LIBC
#include "libioP.h"
ldbl_strong_alias (__vsprintf, _IO_vsprintf)
ldbl_strong_alias (__sprintf,  _IO_sprintf)
ldbl_weak_alias   (__vsprintf, vsprintf)
ldbl_hidden_def   (__sprintf,  sprintf)
ldbl_strong_alias (__sprintf,  sprintf)
#endif
