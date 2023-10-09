#include <stdio.h>
#include "streambuf.h"

int __getc_unlocked(FILE *f)
{
	return getc_unlocked(f);
}

#ifdef _LIBC
#include "libioP.h"
#undef getc_unlocked
weak_alias (__getc_unlocked, getc_unlocked)
weak_alias (__getc_unlocked, fgetc_unlocked)
weak_alias (__getc_unlocked, _IO_getc_unlocked);
libc_hidden_weak (getc_unlocked)
#endif
