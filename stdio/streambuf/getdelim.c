#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include "streambuf.h"

ssize_t __getdelim(char **restrict s, size_t *restrict n, int delim, FILE *restrict f)
{
	char *tmp;
	char *z;
	size_t k;
	size_t i=0;
	int c;
	int unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;

	if (!n || !s) {
		rdstate(f) |= F_ORIENTED | F_ERR;
		if (unlock) __unlockfile(f);
		errno = EINVAL;
		return -1;
	}
	if (!*s) *n=0;

	for (;;) {
		if (rdbuf(f)->gptr != rdbuf(f)->egptr) {
			z = memchr(rdbuf(f)->gptr, delim, rdbuf(f)->egptr - rdbuf(f)->gptr);
			k = z ? z - rdbuf(f)->gptr + 1 : rdbuf(f)->egptr - rdbuf(f)->gptr;
		} else {
			z = 0;
			k = 0;
		}
		if (i+k >= *n) {
			size_t m = i+k+2;
			if (!z && m < SIZE_MAX/4) m += m/2;
			tmp = realloc(*s, m);
			if (!tmp) {
				m = i+k+2;
				tmp = realloc(*s, m);
				if (!tmp) {
					/* Copy as much as fits and ensure no
					 * pushback remains in the FILE buf. */
					k = *n-i;
					memcpy(*s+i, rdbuf(f)->gptr, k);
					rdbuf(f)->gptr += k;
					rdstate(f) |= F_ORIENTED | F_ERR;
					if (unlock) __unlockfile(f);
					errno = ENOMEM;
					return -1;
				}
			}
			*s = tmp;
			*n = m;
		}
		if (k) {
			memcpy(*s+i, rdbuf(f)->gptr, k);
			rdbuf(f)->gptr += k;
			i += k;
		}
		if (z) break;
		if ((c = getc_unlocked(f)) == EOF) {
			if (!i || ~rdstate(f) & F_EOF) {
				if (unlock) __unlockfile(f);
				return -1;
			}
			break;
		}
		/* If the byte read by getc won't fit without growing the
		 * output buffer, push it back for next iteration. */
		if (i+1 >= *n) *--rdbuf(f)->gptr = c;
		else if (((*s)[i++] = c) == delim) break;
	}
	(*s)[i] = 0;
	if (unlock) __unlockfile(f);
	return i;
}

ssize_t __getline(char **restrict s, size_t *restrict n, FILE *restrict f)
{
	return __getdelim(s, n, '\n', f);
}

#ifdef _LIBC
libc_hidden_def (__getdelim)
weak_alias (__getdelim, getdelim)
weak_alias (__getline, getline)
#endif
