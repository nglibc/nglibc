#include <stdio.h>
#include "streambuf.h"

int _IO_feof(FILE *f)
{
	int unlock = (rdstate(f) & F_NEEDLOCK) ? __lockfile(f) : 0;
	int result = !!(rdstate(f) & F_EOF);
	if (unlock) __unlockfile(f);
	return result;
}


#ifdef _LIBC
#include "libioP.h"
weak_alias (_IO_feof, feof)
#endif