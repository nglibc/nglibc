#define _GNU_SOURCE 1
#include <stdio.h>
#include <string.h>
#include "streambuf.h"

#ifdef _LIBC
#undef  fwrite_unlocked
#define fwrite_unlocked(s, l, n, f) _IO_sputn((f), (s), (l)*(n))
size_t _IO_sputn(FILE *, const void *, size_t);
#endif

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

size_t __fwrite_unlocked(const void *src, size_t size, size_t nmemb, FILE *f)
{
	size_t l = size*nmemb, k = __fwritex(src, l, f);
	return k==l ? (size ? nmemb : 0) : k/size;
}

size_t _IO_fwrite(const void *restrict src, size_t size, size_t nmemb, FILE *restrict f)
{
	int unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	size_t l = size*nmemb, k = __fwritex(src, l, f);
	if (unlock) __unlockfile(f);
	return k==l ? (size ? nmemb : 0) : k/size;
}


#ifdef _LIBC
#include "libioP.h"
#undef fwrite_unlocked
weak_alias (_IO_fwrite, fwrite)
weak_alias (__fwrite_unlocked, fwrite_unlocked)
libc_hidden_def  (_IO_fwrite)
libc_hidden_weak (fwrite)
libc_hidden_weak (fwrite_unlocked)
#endif
