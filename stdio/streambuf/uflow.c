#define _GNU_SOURCE 1
#include <stdio.h>
#include "streambuf.h"

#ifdef _LIBC
#define __uflow _IO_uflow
int     __uflow(FILE *);
#endif

/* This function assumes it will never be called if there is already
 * data buffered for reading. */

int __uflowx(FILE *f)
{
	int old = (rdstate(f) >> 16 == 0xFBAD);
	char c;
	if (old) return __uflow(f);
	if (!__toread(f) && rdbuf(f)->virt->read(f, &c, 1)==1) return c;
	return EOF;
}


#ifdef _LIBC
#include "libioP.h"
#undef __uflow
strong_alias (__uflowx, __uflow)
libc_hidden_def (__uflow)
#endif