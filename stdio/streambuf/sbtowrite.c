#include <stdio.h>
#include "streambuf.h"

void __fsetwrite(FILE *f)
{
	rdbuf(f)->pptr = rdbuf(f)->pbase = rdbuf(f)->buf;
	rdbuf(f)->epptr = rdbuf(f)->bufend;
}

int __towrite(FILE *f)
{
	if (rdstate(f) & F_CURWR || rdstate(f) >> 16 == 0xFBAD) return 0;
	rdstate(f) |= F_ORIENTED | F_CURWR;
	if (rdstate(f) & F_NOWR) {
		rdstate(f) |= F_ERR;
		return EOF;
	}
	/* Clear read buffer (easier than summoning nasal demons) */
	rdbuf(f)->gptr = rdbuf(f)->egptr = 0;

	/* Activate write through the buffer. */
	return __fsetwrite(f), 0;
}

//TODO: hidden
void __towrite_needs_stdio_exit(void)
{
	//TODO: __stdio_exit_needed();
}
