#include <wchar.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include "rdstate.h"

#ifndef putc_unlocked
#define putc_unlocked(ch, fx) __overflowx((fx), (ch))
#endif

int     __lockfile  (FILE *);
int     __unlockfile(FILE *);
int     __overflowx (FILE *, int);
size_t  __fwritex   (const char *, size_t, FILE *);

wint_t __fputwc_unlocked(wchar_t c, FILE *f)
{
	/* TODO: Support big endian. (Conversion below assumes little endian) */
	if (~rdstate(f) & F_WIDE) fwide(f, 1);
	if (c < 0x80u) {
		if (putc_unlocked(c, f) != EOF) return c;
	} else {
		unsigned mbc = c>>6 & 0x3F | (c & 0x3F)<<8, sh = 8, len;
		if (c < 0x800u) goto valid_utf8;
		mbc = mbc<<8 | c>>(sh=12) & 0x3F | 0x808080;
		if (c < 0xD800u || c-0xE000u < 0x2000u) goto valid_utf8;
		mbc = mbc<<8 | c>>(sh=18) & 0x3F | 0xF0;
		if (c - 0x10000u < 0x100000u) {
			valid_utf8:
			len = sh / 4; //Dividing sh by 4 is more efficient than dividing by 6
			mbc = mbc | 0x80C0 | (sh==12)<<5;
			if (__fwritex((char *)&mbc, len, f) == len) return c;
		} else errno = EILSEQ;
	}
	rdstate(f) |= F_ERR;
	return WEOF;
}

wint_t fputwc(wchar_t c, FILE *f)
{
	int unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	c = __fputwc_unlocked(c, f);
	if (unlock) __unlockfile(f);
	return c;
}

wint_t putwc(wchar_t c, FILE *f)
{
	return fputwc(c, f);
}

wint_t putwc_unlocked(wchar_t c, FILE *f)
{
	return __fputwc_unlocked(c, f);
}

wint_t putwchar(wchar_t c)
{
	return fputwc(c, stdout);
}

wint_t putwchar_unlocked(wchar_t c)
{
	return __fputwc_unlocked(c, stdout);
}


#ifdef _LIBC
#undef putwc_unlocked
#undef fputwc_unlocked
weak_alias (__fputwc_unlocked, fputwc_unlocked)
libc_hidden_def (putwc)
libc_hidden_def (putwc_unlocked)
#endif
