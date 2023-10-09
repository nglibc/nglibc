#define _GNU_SOURCE 1
#include <stdio.h>
#include <string.h>
#include "streambuf.h"

#define MIN_LEN(a,b) ((a)<(b) ? (a) : (b))

size_t _IO_sgetn(FILE *, void *, size_t);

size_t __fread_unlocked(void *restrict destv, size_t size, size_t nmemb, FILE *restrict f)
{
	unsigned char *dest = destv;
	size_t len = size*nmemb, l = len, k;
	int old = (rdstate(f) >> 16 == 0xFBAD);
	if (old) return size ? _IO_sgetn(f, destv, len) / size : 0;
	if (!size) nmemb = 0;
	rdstate(f) |= F_ORIENTED;

	if (rdbuf(f)->gptr != rdbuf(f)->egptr) {
		/* First exhaust the buffer. */
		k = MIN_LEN(rdbuf(f)->egptr - rdbuf(f)->gptr, l);
		memcpy(dest, rdbuf(f)->gptr, k);
		rdbuf(f)->gptr += k;
		dest += k;
		l -= k;
	}
	
	/* Read the remainder directly */
	for (; l; l-=k, dest+=k) {
		k = __toread(f) ? 0 : rdbuf(f)->virt->read(f, (char *)dest, l);
		if (!k) return (len-l)/size;
	}

	return nmemb;
}

size_t _IO_fread(void *restrict destv, size_t size, size_t nmemb, FILE *restrict f)
{
	int unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	nmemb = __fread_unlocked(destv, size, nmemb, f);
	if (unlock) __unlockfile(f);
	return nmemb;
}


#ifdef _LIBC
#include "libioP.h"
#undef fread_unlocked
weak_alias (_IO_fread, fread)
weak_alias (__fread_unlocked, fread_unlocked)
libc_hidden_def (_IO_fread)
libc_hidden_def (__fread_unlocked)
#endif
