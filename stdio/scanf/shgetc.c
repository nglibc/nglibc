#define _GNU_SOURCE 1
#include <stdio.h>
#include "scanbuf.h"

int  __uflowx(FILE *);

/* The shcnt field stores the number of bytes read so far, offset by
 * the value of buf-gptr at the last function call (__shlim or __shgetc),
 * so that between calls the inline shcnt macro can add gptr-buf to get
 * the actual count. */

void __shlim(scanbuf *f, long long lim)
{
	f->shlim = lim;
	f->shcnt = f->src->buf - f->src->gptr;
	/* If lim is nonzero, egptr must be a valid pointer. */
	if (lim && f->src->egptr - f->src->gptr > lim) f->shend = f->src->gptr + lim;
	else f->shend = f->src->egptr;
}

int __shgetc(scanbuf *f)
{
	long long cnt = shcnt(f);
	int c;
	if (f->shlim && cnt >= f->shlim || (c=__uflowx((FILE *)f->src)) < 0) {
		f->shcnt = f->src->buf - f->src->gptr + cnt;
		f->shend = f->src->gptr;
		f->shlim = -1;
		return -1;
	}
	cnt++;
	if (f->shlim && f->src->egptr - f->src->gptr > f->shlim - cnt)
		f->shend =  f->src->gptr + (f->shlim - cnt);
	else
		f->shend =  f->src->egptr;
	f->shcnt = f->src->buf - f->src->gptr + cnt;
	if (f->src->gptr <= f->src->buf) f->src->gptr[-1] = c;
	return c;
}
