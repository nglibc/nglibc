#define _GNU_SOURCE 1
#ifdef _LIBC
#include <features.h>
#undef  __GLIBC_USE_DEPRECATED_SCANF
#define __GLIBC_USE_DEPRECATED_SCANF 1
#endif
#include <wchar.h>
#include <stdio.h>
#include <stdarg.h>
#include "scanbuf.h"
#include "wcsr2mbs.h"

FILE *__finit   (FILE *, cookie_io_functions_t *, void *, char *, size_t);
void  __setg    (FILE *, char*, char*, char*);
int   __vfwscanf(FILE *, const wchar_t *, va_list);

static ssize_t ws_read(void *f, char *buf, size_t len)
{
	const wchar_t *src = rdbuf(f)->cookie;
	size_t k;

	if (!src) return 0;
	k = wcsr2mbs(rdbuf(f)->buf, &src, rdbuf(f)->bufend-rdbuf(f)->buf);
	if (k==(size_t)-1) {
		rdbuf(f)->gptr = rdbuf(f)->egptr = 0;
		return 0;
	}
	__setg(f, rdbuf(f)->buf, rdbuf(f)->buf, rdbuf(f)->buf+k);
	rdbuf(f)->cookie = (void *)src;

	if (!len || !k) return 0;
	*buf = *rdbuf(f)->gptr++;
	return 1;
}

int __vswscanf(const wchar_t *restrict s, const wchar_t *restrict fmt, va_list ap)
{
	static cookie_io_functions_t iof = { .read = ws_read };
	char buf[256];
	FILE file = { F_WIDE }, *f = __finit(&file, &iof, (wchar_t *)s, buf, sizeof buf);
	return __vfwscanf(f, fmt, ap);
}

int __swscanf(const wchar_t *restrict s, const wchar_t *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = __vswscanf(s, fmt, ap);
	va_end(ap);
	return ret;
}


#ifdef _LIBC
#include "libioP.h"
ldbl_strong_alias (__vswscanf, __isoc99_vswscanf)
ldbl_strong_alias (__swscanf,  __isoc99_swscanf)
ldbl_weak_alias   (__vswscanf, vswscanf)
ldbl_strong_alias (__swscanf,  swscanf)
#endif
