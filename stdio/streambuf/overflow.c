#define _GNU_SOURCE 1
#include <stdio.h>
#include "streambuf.h"

#ifdef _LIBC
#define __overflow _IO_overflow
int     __overflow(FILE *f, int c);
#endif

int __overflowx(FILE *f, int ch)
{
	unsigned char c = ch;
	streambuf *sb = rdbuf(f);
	int old = (rdstate(f) >> 16 == 0xFBAD);
	if (old) return __overflow(f, ch);
	if (sb->epptr == 0 && __towrite(f)) return EOF;
	if (sb->epptr != sb->pptr && c != '\n' && rdstate(f) & F_LBF) return *sb->pptr++ = c;
	if (sb->virt->write(f, (char *)&c, 1)!=1) return EOF;
	return c;
}


#ifdef _LIBC
#include "libioP.h"
#undef __overflow
strong_alias (__overflowx, __overflow)
libc_hidden_def (__overflow)
#endif
