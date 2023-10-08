#ifdef _LIBC
#define _GNU_SOURCE 1
#include <stdio.h>
#include <wchar.h>
#include <limits.h>
#include <malloc/obstack.h>

int __vfprintf_internal (FILE *s, const char *format, va_list ap, unsigned m)
{
	return vfprintf(s, format, ap);
}

int __vfwprintf_internal (FILE *s, const wchar_t *format, va_list ap, unsigned m)
{
        return vfwprintf(s, format, ap);
}

int __vdprintf_internal (int fd, const char *format, va_list ap, unsigned m)
{
	return vdprintf(fd, format, ap);
}

int __vsnprintf_internal (char *string, size_t maxlen, const char *format, va_list ap, unsigned m)          
{
	return vsnprintf(string, maxlen, format, ap);
}

int __vswprintf_internal (wchar_t *string, size_t maxlen, const wchar_t *format, va_list ap, unsigned m)
{
        return vswprintf(string, maxlen, format, ap);
}

int __vsprintf_internal (char *string, size_t maxlen, const char *format, va_list ap, unsigned m)
{
	return vsnprintf(string, maxlen >= 0 ? maxlen : INT_MAX, format, ap);
}

int __vasprintf_internal (char **result_ptr, const char *format, va_list ap, unsigned m)
{
	return vasprintf(result_ptr, format, ap);
}

int __obstack_vprintf_internal (struct obstack *obstack, const char *format, va_list ap, unsigned m)
{
	char buf[1024];
	int len = vsnprintf(buf, sizeof buf, format, ap);
	obstack_grow(obstack, buf, len);
	return len;
}


#include "libioP.h"
#include <array_length.h> 
#include <math_ldbl_opt.h>
#include <printf.h>
#include <stdio_ext.h>
#include <unistd.h>
#include <printf_buffer.h>

void __printf_buffer_flush_snprintf (struct __printf_buffer_snprintf *buf)
{
	fputs("printf_buffer_flush_snprintf not implemented\n", stderr);
	abort();
}

void __printf_buffer_flush_dprintf (struct __printf_buffer_dprintf *buf)
{
	fputs("printf_buffer_flush_dprintf not implemented\n", stderr);
	abort();
}

void __printf_buffer_flush_asprintf (struct __printf_buffer_asprintf *buf)
{
	fputs("printf_buffer_flush_asprintf not implemented\n", stderr);
	abort();
}

void __printf_buffer_flush_obstack (struct __printf_buffer_obstack *buf)
{
	fputs("printf_buffer_flush_obstack not implemented\n", stderr);
	abort();
}
#endif
