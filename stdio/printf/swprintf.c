#define _GNU_SOURCE 1
#include <wchar.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include "mb2wc.h"

typedef struct {
	wchar_t *ws;
	size_t l;
} cookie;

FILE *__finit    (FILE *, cookie_io_functions_t *, void *, char *, size_t);
void *__fcookie  (FILE *);
void  __fsetwrite(FILE *);
void  __fseterror(FILE *);
int   __pflush   (FILE *, const void *);
int   __vfwprintf(FILE *, const wchar_t *, va_list);

static ssize_t sw_write(void *f, const char *s, size_t l)
{
	size_t all = l;
	int i = 0;
	cookie *c = __fcookie(f);
	if (__pflush(f, s) == -1) return -1;
	while (c->l && l && (i = mbtowc(c->ws, s)) >= 0) {
		if (!i) i=1;
		s+=i;
		l-=i;
		c->l--;
		c->ws++;
	}
	*c->ws = 0;
	return (i < 0) ?  __fseterror(f),i: __fsetwrite(f),all;
}

int __vswprintf(wchar_t *restrict s, size_t n, const wchar_t *restrict fmt, va_list ap)
{
	static cookie_io_functions_t iof = { .write = sw_write };
	cookie c = { s, n-1 };
	char buf[256];  // last 4 bytes reserved for padding needed by mb2wc
	if (n > INT_MAX) return (void)(errno = EOVERFLOW), -1;
	if (!n) return -1; else {
		FILE file = {}, *f = __finit(&file, &iof, &c, buf, sizeof buf-4);
		int ret = __vfwprintf(f, fmt, ap);
		sw_write(f, 0, 0);
		return ret >= n ? -1 : ret;
	}
}

int __swprintf(wchar_t *restrict s, size_t n, const wchar_t *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = __vswprintf(s, n, fmt, ap);
	va_end(ap);
	return ret;
}


#ifdef _LIBC
#include "libioP.h"
ldbl_weak_alias   (__vswprintf, vswprintf)
ldbl_strong_alias (__swprintf,  swprintf)
#endif
