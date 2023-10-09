#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "streambuf.h"
#ifdef _LIBC
#include "libioP.h"
#endif

size_t _IO_getline(FILE *f, char *s, size_t n, int delim, int extract_delim)
{
	size_t i = 0;
	int c = EOF-1, unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	while (i < n && (c = getc_unlocked(f)) != EOF && c != delim) s[i++] = c;
	if (c == delim) {
		if (extract_delim > 0 && i < n) s[i++] = c;
		else if (extract_delim < 0) ungetc(c, f);
	} else if (!extract_delim && (~rdstate(f) & F_EOF || !i)) i = -1;
	if (unlock) __unlockfile(f);
	return i;
}

char *__gets(char *s)
{
	size_t i = _IO_getline(stdin, s, INT_MAX, '\n', 0);
	if (i+1 == 0) return NULL; else s[i] = 0;
	return s;
}


#ifdef _LIBC
libc_hidden_def (_IO_getline)
weak_alias (__gets, gets)
#endif
