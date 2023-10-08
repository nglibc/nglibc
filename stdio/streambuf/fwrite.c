#define _GNU_SOURCE 1
#include <stdio.h>
#include <string.h>
#include "streambuf.h"

size_t __fwritex(const char *restrict s, size_t l, FILE *restrict f)
{
	size_t i = 0, all = l;
	int old = (rdstate(f) >> 16 == 0xFBAD);
	if (old) return fwrite_unlocked(s, 1, l, f);
	if (__towrite(f)) return 0;

	if (l > rdbuf(f)->epptr - rdbuf(f)->pptr) {
		if (rdbuf(f)->virt->write) return rdbuf(f)->virt->write(f, s, l);
		l = rdbuf(f)->epptr - rdbuf(f)->pptr;
	}

	if (rdstate(f) & F_LBF && rdbuf(f)->virt->write) {
		/* Match /^(.*\n|)/ */
		for (i=l; i && s[i-1] != '\n'; i--);
		if (i) {
			size_t n = rdbuf(f)->virt->write(f, s, i);
			if (n < i) return n;
			s += i;
			l -= i;
		}
	}

	memcpy(rdbuf(f)->pptr, s, l);
	rdbuf(f)->pptr += l;
	return all; // successful write reported even if output to mem buf truncated
}

