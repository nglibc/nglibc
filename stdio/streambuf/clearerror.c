#include <stdio.h>
#include "streambuf.h"

void __clearerr(FILE *f)
{
	int unlock = (rdstate(f) & F_NEEDLOCK) ? __lockfile(f) : 0;
	rdstate(f) &= ~(F_EOF|F_ERR);
	if (unlock) __unlockfile(f);
}


#ifdef _LIBC
weak_alias (__clearerr, clearerr)
#endif
