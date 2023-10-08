#include <sys/uio.h>
#include <stdio.h>

void   __fsetwrite(FILE *);
void   __fseterror(FILE *);
void  *__fcookie  (FILE *);
size_t __fpending (FILE *);
char  *__pbase    (FILE *);

ssize_t __stdio_write(void *f, const char *buf, size_t len)
{
	struct iovec iovs[2] = {
		{ .iov_base = __pbase(f), .iov_len = __fpending(f) },
		{ .iov_base = (void *)buf, .iov_len = len }
	};
	struct iovec *iov = iovs;
	size_t rem = iov[0].iov_len + iov[1].iov_len;
	int    iovcnt = 2, fd = *(int *)__fcookie(f);;
	ssize_t cnt;
	for (;;) {
		cnt = writev(fd, iov, iovcnt);
		if (cnt == rem) return __fsetwrite(f), len;
		if (cnt  < 0)   return __fseterror(f), iovcnt == 2 ? 0 : len-iov[0].iov_len;
		rem -= cnt;
		if (cnt > iov[0].iov_len) {
			cnt -= iov[0].iov_len;
			iov++; iovcnt--;
		}
		iov[0].iov_base = (char *)iov[0].iov_base + cnt;
		iov[0].iov_len -= cnt;
	}
}
