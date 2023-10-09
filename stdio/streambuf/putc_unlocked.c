#include <stdio.h>
#include "streambuf.h"

int __putc_unlocked(int c, FILE *f)
{
	return putc_unlocked(c, f);
}

#ifdef _LIBC
#include "libioP.h"
#undef putc_unlocked
#undef fputc_unlocked
weak_alias (__putc_unlocked, putc_unlocked)
weak_alias (__putc_unlocked, fputc_unlocked);
weak_alias (__putc_unlocked, _IO_putc_unlocked);
libc_hidden_weak (putc_unlocked)
libc_hidden_def (fputc_unlocked)
#endif
