#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdio_ext.h>
#include "streambuf.h"

#undef __fsetlocking
int __fsetlocking(FILE *f, int type)
{
	return 0;
}

int __fwriting(FILE *f)
{
	return (rdstate(f) & F_NORD) || (rdstate(f) & F_CURWR);
}

int __freading(FILE *f)
{
	return (rdstate(f) & F_NOWR) || !__fwriting(f) && rdbuf(f)->egptr;
}

int __freadable(FILE *f)
{
	return !(rdstate(f) & F_NORD);
}

int __fwritable(FILE *f)
{
	return !(rdstate(f) & F_NOWR);
}

int __flbf(FILE *f)
{
	return (rdstate(f) & F_LBF);
}

size_t __fbufsize(FILE *f)
{
	return rdbuf(f)->bufend - rdbuf(f)->buf;
}

#ifdef _LIBC
libc_hidden_def (__fsetlocking)
#endif
