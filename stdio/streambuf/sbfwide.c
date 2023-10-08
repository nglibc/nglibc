#include <wchar.h>
#include <stdio.h>
#include "streambuf.h"

#undef _IO_fwide
int _IO_fwide(FILE *, int);

int fwide(FILE *f, int mode)
{
#ifdef _LIBC
	/* IO_fwide should not be called directly as setting mode will corrupt FILE/streambuf
	   struct & reading mode always returns zero (if new FILE struct is zero initialized)
	   Note: glibc misc & argp uses IO_fwide to read mode, but doesn't set mode. */  
	int old = (rdstate(f) >> 16 == 0xFBAD);
	if (old) return _IO_fwide(f, mode);
#endif
	int unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	if (mode && ~rdstate(f) & F_ORIENTED) {
		rdstate(f) |= mode>0 ? (F_ORIENTED|F_WIDE) : F_ORIENTED;
	}
	mode = (rdstate(f) & F_ORIENTED) ? !!(rdstate(f) & F_WIDE)*2-1 : 0;
	if (unlock) __unlockfile(f);
	return mode;
}
