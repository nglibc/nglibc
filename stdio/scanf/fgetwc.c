#define _GNU_SOURCE 1
#include <wchar.h>
#include <stdio.h>
#include <errno.h>
#include "scanbuf.h"

#undef  getc_unlocked
#define getc_unlocked(f)  (rdbuf(f)->gptr != rdbuf(f)->egptr ? (unsigned char)*rdbuf(f)->gptr++ : __uflowx(f))
#define mbrtowc           __mbr2wc

size_t  __mbr2wc    (wchar_t *, const char *, size_t, unsigned *);
int     __lockfile  (FILE *);
int     __unlockfile(FILE *);
int     __uflowx    (FILE *f);

wint_t __fgetwc_unlocked(FILE *f)
{
	wchar_t wc;
	size_t l;
	int c;
	unsigned char b;
	unsigned st0 = 0, st = 0;
	int first = 1;
	if (~rdstate(f) & F_WIDE) fwide(f, 1);

	/* Convert character from buffer if possible */
	if (rdbuf(f)->gptr != rdbuf(f)->egptr) {
		l = mbrtowc(&wc, (void *)rdbuf(f)->gptr, rdbuf(f)->egptr - rdbuf(f)->gptr, &st0);
		if (l+1 >= 1) {
			rdbuf(f)->gptr += l + !l; // l==0 means 1 byte, null
			return wc;
		}
	}

	/* Convert character byte-by-byte */
	do {
		b = c = getc_unlocked(f);
		if (c < 0) {
			if (!first) {
				rdstate(f) |= F_ERR;
				errno = EILSEQ;
			}
			return WEOF;
		}
		l = mbrtowc(&wc, (void *)&b, 1, &st);
		if (l == -1) {
			if (!first) {
				rdstate(f) |= F_ERR;
				ungetc(b, f);
			}
			return WEOF;
		}
		first = 0;
	} while (l == -2);

	return wc;
}

wint_t _IO_getwc(FILE *f)
{
	wint_t c;
	int unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	c = __fgetwc_unlocked(f);
	if (unlock) __unlockfile(f);
	return c;
}

wint_t getwchar_unlocked(void)
{
	return __fgetwc_unlocked(stdout);
}

wint_t getwchar(void)
{
	return fgetwc(stdout);
}


#ifdef _LIBC
#undef getwc_unlocked
weak_alias (__fgetwc_unlocked, getwc_unlocked)
weak_alias (__fgetwc_unlocked, fgetwc_unlocked)

#undef getwc
weak_alias (_IO_getwc, getwc)
weak_alias (_IO_getwc, fgetwc)
#endif
