#include <stdio.h>
#include <stdarg.h>

int __vfprintf(FILE *restrict, const char *restrict, va_list);
	
int __fprintf(FILE *restrict f, const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = __vfprintf(f, fmt, ap);
	va_end(ap);
	return ret;
}


#ifdef _LIBC
#include "libioP.h"
ldbl_weak_alias   (__fprintf,  _IO_fprintf)
ldbl_hidden_def   (__fprintf,  fprintf)
ldbl_strong_alias (__fprintf,  fprintf)
#endif
