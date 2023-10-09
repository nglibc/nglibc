#define fsetpos64 __disabled_fsetpos64
#define fgetpos64 __disabled_fgetpos64
#include <stdio.h>
#include "streambuf.h"

int _IO_new_fsetpos(FILE *f, const fpos_t *pos)
{
	return fseeko(f, *(const long long *)pos, SEEK_SET);
}

int _IO_new_fgetpos(FILE *restrict f, fpos_t *restrict pos)
{
	off_t off = ftello(f);
	if (off < 0) return -1;
	*(long long *)pos = off;
	return 0;
}


#ifdef _LIBC
#include "libioP.h"
#undef fsetpos64
#undef fgetpos64
strong_alias     (_IO_new_fsetpos, __fsetpos64)
strong_alias     (_IO_new_fgetpos, __fgetpos64)
versioned_symbol (libc, _IO_new_fgetpos, fgetpos, GLIBC_2_2);
versioned_symbol (libc, _IO_new_fsetpos, fsetpos, GLIBC_2_2);
versioned_symbol (libc, __fsetpos64, fsetpos64, GLIBC_2_2);
versioned_symbol (libc, __fgetpos64, fgetpos64, GLIBC_2_2);
#endif
