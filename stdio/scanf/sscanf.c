#define _GNU_SOURCE 1
#ifdef _LIBC
#include <features.h>
#undef  __GLIBC_USE_DEPRECATED_SCANF
#define __GLIBC_USE_DEPRECATED_SCANF 1
#endif
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "scanbuf.h"

FILE *__finit  (FILE *, cookie_io_functions_t *, void *, char *, size_t);
void  __setg   (FILE *, char*, char*, char*);
int   __vfscanf(FILE *, const char *, va_list);

static ssize_t s_read(void *f, char *buf, size_t len)
{
	char *src = rdbuf(f)->cookie;
	size_t k = len+256;
	char *end = memchr(src, 0, k);
	if (end) k = end-src;
	if (k < len) len = k;
	memcpy(buf, src, len);
	__setg(f, src+len, src+len, src+k);
	rdbuf(f)->cookie = src+k;
	return len;
}

int __vsscanf(const char *restrict s, const char *restrict fmt, va_list ap)
{
	static cookie_io_functions_t iof = { .read = s_read };
	FILE file = {}, *f = __finit(&file, &iof, (char *)s, (char *)s, 0);
	return __vfscanf(f, fmt, ap);
}

int __sscanf(const char *restrict s, const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = __vsscanf(s, fmt, ap);
	va_end(ap);
	return ret;
}


#ifdef _LIBC
#include "libioP.h"
#undef vsscanf
#undef sscanf
ldbl_weak_alias   (__vsscanf, _IO_vsscanf)
ldbl_strong_alias (__sscanf,  _IO_sscanf)
ldbl_strong_alias (__vsscanf, __isoc99_vsscanf)
ldbl_strong_alias (__sscanf,  __isoc99_sscanf)
libc_hidden_def   (__isoc99_vsscanf)
libc_hidden_def   (__isoc99_sscanf)
ldbl_weak_alias   (__vsscanf, vsscanf)
ldbl_strong_alias (__sscanf,  sscanf)
#endif
