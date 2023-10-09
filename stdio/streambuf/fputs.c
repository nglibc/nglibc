#include <stdio.h>
#include <string.h>

size_t __fwritex (const char *, size_t, FILE *);
size_t _IO_fwrite(const void *, size_t, size_t, FILE *);

int __fputs_unlocked(const char *restrict s, FILE *restrict f)
{
	size_t l = strlen(s);
	return (__fwritex(s, l, f)==l) - 1;
}

int _IO_fputs(const char *restrict s, FILE *restrict f)
{
	size_t l = strlen(s);
	return (_IO_fwrite(s, 1, l, f)==l) - 1;
}

#ifdef _LIBC
#include "libioP.h"
weak_alias (__fputs_unlocked, fputs_unlocked)
weak_alias (_IO_fputs, fputs)
libc_hidden_def  (__fputs_unlocked)
libc_hidden_def  (_IO_fputs)
libc_hidden_weak (fputs)
libc_hidden_weak (fputs_unlocked)
#endif
