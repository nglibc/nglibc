#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/uio.h>

FILE   *__finit      (FILE *, cookie_io_functions_t *, void *, char *, size_t);
void   *__fcookie    (FILE *);
int     __vfprintf   (FILE *, const char *, va_list);
ssize_t __stdio_write(void *, const char *, size_t);

int __vdprintf(int fd, const char *restrict fmt, va_list ap)
{
	static cookie_io_functions_t iof = { .write = __stdio_write };
	FILE file = {}, *f = __finit(&file, &iof, &fd, (char *)fmt, 0);
	return vfprintf(f, fmt, ap);
}

int __dprintf(int fd, const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = __vdprintf(fd, fmt, ap);
	va_end(ap);
	return ret;
}

#include "../streambuf/streambuf.h"
#include "../bits/types/struct_FILE.h"

#ifdef _LIBC
#include "libioP.h"
ldbl_weak_alias (__vdprintf, vdprintf)
libc_hidden_def (__dprintf)
ldbl_hidden_def (__dprintf,  dprintf)
ldbl_weak_alias (__dprintf,  dprintf)
#endif
