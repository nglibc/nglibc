#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdio_ext.h>
#include "streambuf.h"

size_t __freadahead(FILE *);

void __fpurge(FILE *f)
{
	rdbuf(f)->pptr = rdbuf(f)->pbase = rdbuf(f)->epptr = 0;
	rdbuf(f)->gptr = rdbuf(f)->egptr = 0;
	rdstate(f) &= ~F_CURWR;
}

int __fflushx(FILE *f)
{
	ssize_t delta;
	
	/* If writing, flush output */
	if (__fpending(f)) {
		rdbuf(f)->virt->write(f, 0, 0);
		if (!rdbuf(f)->pptr) return EOF;
	}

	/* If reading, sync position, per POSIX */
	delta = -__freadahead(f);
	if (delta) rdbuf(f)->virt->seek(f, &delta, SEEK_CUR);

	/* Clear read and write modes */
	__fpurge(f);
	return 0;
}

#ifdef _LIBC
weak_alias (__fpurge, fpurge);
#endif
