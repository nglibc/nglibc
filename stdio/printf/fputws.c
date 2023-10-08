#include <wchar.h>
#include <stdio.h>
#include <errno.h>
#include "rdstate.h"
#include "wcsr2mbs.h"

int     __lockfile  (FILE *f);
int     __unlockfile(FILE *f);
size_t  __fwritex   (const char *, size_t, FILE *);

int fputws_unlocked(const wchar_t *ws, FILE *f)
{
	char buf[BUFSIZ];
	size_t l = 0;
	if (~rdstate(f) & F_WIDE) fwide(f, 1);
	while (ws && (l = wcsr2mbs(buf, &ws, sizeof buf))+1 > 1) {
		if (__fwritex(buf, l, f) < l) return -1;
	}
	return l; /* 0 or -1 */
}

int fputws(const wchar_t *restrict ws, FILE *restrict f)
{
	int unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	size_t l = fputws_unlocked(ws, f);
	if (unlock) __unlockfile(f);
	return l;
}


#ifdef _LIBC
libc_hidden_def (fputws_unlocked)
#endif
