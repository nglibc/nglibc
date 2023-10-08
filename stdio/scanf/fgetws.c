#include <wchar.h>
#include <stdio.h>
#include "scanbuf.h"

int     __lockfile  (FILE *);
int     __unlockfile(FILE *);
wint_t  __fgetwc_unlocked(FILE *);

size_t _IO_getwline(FILE *f, wchar_t *s, size_t n, wint_t delim, int extract_delim)
{
	size_t i = 0;
	wint_t c;
	int unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	while (i < n && (c = __fgetwc_unlocked(f)) != WEOF) {
		if (c != delim) s[i++] = c; else {
			if (extract_delim > 0) s[i++] = c;
			else if (extract_delim < 0) ungetwc(c, f);
			break;
		}
	}
	if (unlock) __unlockfile(f);
	return i;
}

wchar_t *__fgetws(wchar_t *restrict s, int n, FILE *restrict f)
{
	size_t i;
	if (n > 0) s[i = _IO_getwline(f, s, n-1, L'\n', 1)] = 0; else return s;
	return rdstate(f) & F_ERR || !i ? NULL : s;
}


#ifdef _LIBC
weak_alias (__fgetws, fgetws)
#endif
