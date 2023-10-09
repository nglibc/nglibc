#define _GNU_SOURCE 1
#define __fseeko64 __fseeko64_disable
#include <stdio.h>
#include <errno.h>
#include "streambuf.h"

off_t _IO_seekoff(FILE *, off_t, int, int);

int __fseeko_unlocked(FILE *f, off_t off, int whence)
{
	int old = (rdstate(f) >> 16 == 0xFBAD);
	if (old) return _IO_seekoff(f,off,whence,3)==-1 ? EOF : 0;

	/* Fail immediately for invalid whence argument. */
	if (whence != SEEK_CUR && whence != SEEK_SET && whence != SEEK_END) {
		errno = EINVAL;
		return -1;
	}

	/* Adjust relative offset for unread data in buffer, if any. */
	if (whence == SEEK_CUR && rdbuf(f)->egptr) off -= rdbuf(f)->egptr - rdbuf(f)->gptr;

	/* Flush write buffer, and report error on failure. */
	if (rdbuf(f)->pptr != rdbuf(f)->pbase) {
		rdbuf(f)->virt->write(f, 0, 0);
		if (!rdbuf(f)->pptr) return -1;
	}

	/* Leave writing mode */
	rdbuf(f)->pptr = rdbuf(f)->pbase = rdbuf(f)->epptr = 0;
	/* Perform the underlying seek. */
	if (rdbuf(f)->virt->seek(f, &off, whence) < 0) return -1;

	/* If seek succeeded, file is seekable and we discard read buffer. */
	rdbuf(f)->gptr = rdbuf(f)->egptr = 0;
	rdstate(f) &= ~F_EOF;
	return 0;
}

int __fseeko(FILE *f, off_t off, int whence)
{
	int unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	int result = __fseeko_unlocked(f, off, whence);
	if (unlock) __unlockfile(f);
	return result;
}

int fseek(FILE *f, long off, int whence)
{
	return __fseeko(f, off, whence);
}

void rewind(FILE *f)
{
	int unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	__fseeko_unlocked(f, 0, SEEK_SET);
	rdstate(f) &= ~F_ERR;
	if (unlock) __unlockfile(f);
}


#ifdef _LIBC
#include "libioP.h"
#undef  __fseeko64
weak_alias   (__fseeko, fseeko)
weak_alias   (__fseeko, fseeko64)
strong_alias (__fseeko, __fseeko64)
libc_hidden_def (fseek)
libc_hidden_def (rewind)
libc_hidden_ver (__fseeko, __fseeko64)
#endif
