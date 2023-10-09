#define _GNU_SOURCE 1
#include <stdio.h>
#include "streambuf.h"

/* The behavior of this function is undefined except when it is the first
 * operation on the stream, so the presence or absence of locking is not
 * observable in a program whose behavior is defined. Thus no locking is
 * performed here. No allocation of buffers is performed, but a buffer
 * provided by the caller is used as long as it is suitably sized. */

int setvbuf(FILE *restrict f, char *restrict buf, int type, size_t size)
{
	rdstate(f) &= ~F_LBF;
	if (type == _IONBF) {
		rdbuf(f)->bufend = rdbuf(f)->buf;
	} else if (type == _IOLBF || type == _IOFBF) {
		if (buf && size > UNGET) {
			rdbuf(f)->buf = (void *)(buf + UNGET);
			rdbuf(f)->bufend = (void *)(buf + size);
			if (type == _IOLBF) rdstate(f) |= F_LBF;
		}
	} else return -1;
	rdstate(f) |= F_SVB;
	return 0;
}

void (setbuf)(FILE *restrict f, char *restrict buf)
{
	setvbuf(f, buf, buf ? _IOFBF : _IONBF, BUFSIZ);
}

void (setbuffer)(FILE *f, char *buf, size_t size)
{
	setvbuf(f, buf, buf ? _IOFBF : _IONBF, size);
}

void (setlinebuf)(FILE *f)
{
	setvbuf(f, 0, _IOLBF, 0);
}

