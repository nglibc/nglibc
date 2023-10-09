#include <stdio.h>
#include <stdio_ext.h>
#include "streambuf.h"

#if 0 //TODO:
const char *__freadptr(FILE *f, size_t *sizep)
{
	if (f->gptr == f->egptr) return 0;
	*sizep = f->egptr - f->gptr;
	return (const char *)f->gptr;
}

void __freadptrinc(FILE *f, size_t inc)
{
	rdbuf(f)->gptr += inc;
}

void __fseterr(FILE *f)
{
	rdstate(f) |= F_ERR;
}
#endif
