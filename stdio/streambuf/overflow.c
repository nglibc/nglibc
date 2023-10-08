#define _GNU_SOURCE 1
#include <stdio.h>
#include "streambuf.h"

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
