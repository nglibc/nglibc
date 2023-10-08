#include <wchar.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include "scanbuf.h"
#include "wc2mb.h"

int __lockfile  (FILE *f);
int __unlockfile(FILE *f);
int __toread    (FILE *);

wint_t __ungetwc(wint_t c, FILE *f)
{
	unsigned char mbc[MB_LEN_MAX];
	int l, unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	if (~rdstate(f) & F_WIDE) fwide(f, 1);
	if (!__freading(f) && __toread(f)) return WEOF;
	if (c == WEOF || (l = wctomb((void *)mbc, c)) < 0) return WEOF;
	if (isascii(c)) ungetc(c, f);
	else while (--l >= 0) ungetc(mbc[l], f);
	rdstate(f) &= ~F_EOF;
	if (unlock) __unlockfile(f);
	return c;
}


#ifdef _LIBC
#undef ungetwc
weak_alias (__ungetwc, ungetwc)
#endif
