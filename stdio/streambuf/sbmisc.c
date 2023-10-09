#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdio_ext.h>
#include "streambuf.h"

FILE *__finit(FILE *f, cookie_io_functions_t *iof, void *c, char *buf, size_t n)
{
	rdbuf(f)->virt   = iof;
	rdbuf(f)->cookie = c;
	rdbuf(f)->buf    = buf;
	rdbuf(f)->bufend = buf + n;
	rdbuf(f)->lock   = &rdbuf(f)->lock_data;
	return f;
}

void *__fcookie(FILE *f)
{
	return rdbuf(f)->cookie;
}

void __fseterror(FILE *f)
{
	rdbuf(f)->pptr = rdbuf(f)->pbase = rdbuf(f)->epptr = 0;
	rdstate(f) = rdstate(f) & ~F_CURWR | F_ERR;
}

size_t __fpending(FILE *f)
{
        return rdbuf(f)->epptr ? rdbuf(f)->pptr - rdbuf(f)->pbase : 0;
}

size_t __freadahead(FILE *f)
{
	return rdbuf(f)->egptr ? rdbuf(f)->egptr - rdbuf(f)->gptr : 0;
}

char *__pbase(FILE *f)
{
        return rdbuf(f)->pbase;
}

int   __pflush(FILE *f, const void *buf)
{
        if (buf != rdbuf(f)->pbase) {
                size_t n = rdbuf(f)->pptr - rdbuf(f)->pbase;
                return rdbuf(f)->virt->write(f, rdbuf(f)->pbase, n);
        }
        return 0;
}