#define _GNU_SOURCE 1
#define __ftello64 __ftello64_disable
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include "streambuf.h"

long _IO_ftell(FILE *f);

off_t __ftello_unlocked(FILE *f)
{
	int old = (rdstate(f) >> 16 == 0xFBAD);
	if (old) return (off_t) _IO_ftell(f);
	off_t pos = 0;
	int ret = rdbuf(f)->virt->seek(f, &pos,
		(rdstate(f) & F_APP) && rdbuf(f)->pptr != rdbuf(f)->pbase
		? SEEK_END : SEEK_CUR);
	if (ret < 0) return ret;

	/* Adjust for data in buffer. */
	if (rdbuf(f)->egptr)       pos += rdbuf(f)->gptr - rdbuf(f)->egptr;
	else if (rdbuf(f)->pbase) pos += rdbuf(f)->pptr - rdbuf(f)->pbase;
	return pos;
}

off_t __ftello(FILE *f)
{
	int unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	off_t pos  = __ftello_unlocked(f);
	if (unlock) __unlockfile(f);
	return pos;
}

long ftell(FILE *f) //TODO: _IO_ftell(FILE *f)
{
	off_t pos = __ftello(f);
	if (pos > LONG_MAX) return (void)(errno = EOVERFLOW), -1;
	else return pos;
}


#ifdef _LIBC
#include "libioP.h"
#undef  __ftello64
//weak_alias   (_IO_ftell, ftell)
weak_alias   (__ftello, ftello)
weak_alias   (__ftello, ftello64)
strong_alias (__ftello, __ftello64)
//libc_hidden_def (_IO_ftell)
libc_hidden_def (__ftello)
libc_hidden_ver (__ftello, __ftello64)
#endif
