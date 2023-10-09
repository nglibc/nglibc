#include <stdio.h>
#include <string.h>
#include "streambuf.h"

#define MIN_LEN(a,b) ((a)<(b) ? (a) : (b))

char *__fgets_unlocked(char *restrict s, int n, FILE *restrict f)
{
	char *p = s;
	char *z;
	size_t k;
	int c;
	if (n > 1) n--; else {
		rdstate(f) |= F_ORIENTED;
		if (n<1) return 0;
		*s = 0;
		return s;
	}

	while (n) {
		if (rdbuf(f)->gptr != rdbuf(f)->egptr) {
			z = memchr(rdbuf(f)->gptr, '\n', rdbuf(f)->egptr - rdbuf(f)->gptr);
			k = z ? z - rdbuf(f)->gptr + 1 : rdbuf(f)->egptr - rdbuf(f)->gptr;
			k = MIN_LEN(k, n);
			memcpy(p, rdbuf(f)->gptr, k);
			rdbuf(f)->gptr += k;
			p += k;
			n -= k;
			if (z || !n) break;
		}
		if ((c = getc_unlocked(f)) < 0) {
			if (p==s || ~rdstate(f) & F_EOF) s = 0;
			break;
		}
		n--;
		if ((*p++ = c) == '\n') break;
	}
	if (s) *p = 0;
	return s;
}

char *_IO_fgets(char *restrict s, int n, FILE *restrict f)
{
	int unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	s = __fgets_unlocked(s, n, f);
	if (unlock) __unlockfile(f);
	return s;
}


#ifdef _LIBC
#include "libioP.h"
weak_alias (_IO_fgets, fgets)
libc_hidden_def (__fgets_unlocked)
weak_alias (__fgets_unlocked, fgets_unlocked)
libc_hidden_weak (fgets_unlocked)
#endif
