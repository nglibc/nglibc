#include <stdio.h>
#include "streambuf.h"

int (_IO_getc)(FILE *f)
{
	int ret;
	int unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	ret = getc_unlocked (f);
	if (unlock) __unlockfile(f);
	return ret;
}

int (getchar)(void)
{
	return (_IO_getc)(stdout);
}


#ifdef _LIBC
#include "libioP.h"
#undef getc
#undef _IO_getc
weak_alias (_IO_getc, getc)
weak_alias (_IO_getc, fgetc)
#endif
