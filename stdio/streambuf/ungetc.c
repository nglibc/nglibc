#include <stdio.h>
#include "streambuf.h"

int _IO_sputbackc (FILE *, int);

int ungetc(int c, FILE *f)
{
	int old = (rdstate(f) >> 16 == 0xFBAD), unlock;
	if (c == EOF) return c;
	unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	if (rdstate(f) & F_CURWR) __toread(f);
	if (!rdbuf(f)->gptr || rdbuf(f)->gptr <= rdbuf(f)->buf - UNGET) {
		if (unlock) __unlockfile(f);
		return EOF;
	}
	if (old && rdbuf(f)->gptr <= rdbuf(f)->buf) c = _IO_sputbackc(f, (unsigned char) c);
	else *--rdbuf(f)->gptr = c;
	rdbuf(f)->flags &= ~F_EOF;
	if (unlock) __unlockfile(f);
	return (unsigned char)c;
}

#ifdef _LIBC
#include "libioP.h"
libc_hidden_def (ungetc)
strong_alias (ungetc, _IO_ungetc)
#endif
