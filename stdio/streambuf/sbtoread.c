#define _GNU_SOURCE 1
#include <stdio.h>
#include "streambuf.h"

int __toread(FILE *f)
{
	int old = (rdstate(f) >> 16 == 0xFBAD);
	if (old) return rdstate(f) & F_NORD;

	/* Flush write buffer (if any) */
	if (rdbuf(f)->pptr != rdbuf(f)->pbase) rdbuf(f)->virt->write(f, 0, 0);
	rdbuf(f)->pptr = rdbuf(f)->pbase = rdbuf(f)->epptr = 0;
	/* Clear current writing flag and check for no read */
	rdstate(f) = rdstate(f) & ~F_CURWR | F_ORIENTED;
	if (rdstate(f) & F_NORD) {
		rdstate(f) |= F_ERR;
		return EOF;
	}
	/* Set read pointers to underflow position */
	rdbuf(f)->gptr = rdbuf(f)->egptr = rdbuf(f)->bufend;
	return (rdstate(f) & F_EOF) ? EOF : 0;
}

void __setg (FILE *f, char* eback, char* gptr, char* egptr)
{
	rdbuf(f)->eback = eback;
	rdbuf(f)->gptr  = gptr;
	rdbuf(f)->egptr = egptr;
}

//TODO: hidden
void __toread_needs_stdio_exit(void)
{
	//TODO:__stdio_exit_needed();
}
