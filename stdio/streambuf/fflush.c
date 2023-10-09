#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdio_ext.h>
#include "streambuf.h"
#define stub_alias(old, new) \
	extern __typeof(old) new __attribute__((__weak__, __alias__(#old)))

extern streambuf __ofl_head;
extern int       __fflushx(FILE *);

/* stdout.c will override this if linked */
static FILE *volatile dummy = 0;
stub_alias (dummy, __stdout_used);
stub_alias (dummy, __stderr_used);

int __fflush(FILE *f)
{
	int _IO_flush_all(void);
	int old = f && (rdstate(f) >> 16 == 0xFBAD);
	if (old) return _IO_flush_all();

	if (!f) {
		int r = 0;
		r |= _IO_flush_all();
		if (__stdout_used) r |= fflush(__stdout_used);
		if (__stderr_used) r |= fflush(__stderr_used);

		int unlock = __lockfile((FILE *)&__ofl_head);
		for (f = __ofl_head.next; rdbuf(f) != &__ofl_head; f = rdbuf(f)->next) {
			int unlock  = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
			int pending = __fpending(f);
			if (pending) r |= __fflushx(f);
			if (unlock)  __unlockfile(f);
		}
		if (unlock) __unlockfile((FILE *)&__ofl_head);
		return r;
	} else {
		int unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
		int ret    = __fflushx(f);
		if (unlock) __unlockfile(f);
		return ret;
	}
}

void _flushlbf(void)
{
	fflush(0);
}

#ifdef _LIBC
#include "libioP.h"
weak_alias   (__fflush, fflush)
weak_alias   (__fflush, fflush_unlocked)
strong_alias (__fflush, _IO_fflush)
strong_alias (__fflush, __fflush_unlocked)
libc_hidden_def  (_IO_fflush)
libc_hidden_def  (__fflush_unlocked)
libc_hidden_weak (fflush)
libc_hidden_weak (fflush_unlocked)          
#endif

