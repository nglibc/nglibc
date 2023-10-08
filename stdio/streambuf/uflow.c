#define _GNU_SOURCE 1
#include <stdio.h>
#include "streambuf.h"

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

