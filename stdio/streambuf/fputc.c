#include <stdio.h>
#include "streambuf.h"

int (_IO_putc)(int c, FILE *f)
{
	int unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	c = putc_unlocked(c, f);
	if (unlock) __unlockfile(f);
	return c;
}

int (putchar)(int c)
{
	return (_IO_putc)(c, stdout);
}


#ifdef _LIBC
#include "libioP.h"
#undef putc
#undef _IO_putc
libc_hidden_def (_IO_putc)
weak_alias (_IO_putc, putc)
weak_alias (_IO_putc, fputc)
#endif
