#define strfromf32  __hide_strfromf32
#define strfromf32x __hide_strfromf32x
#define strfromf64  __hide_strfromf64
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
					
int strfromf(char *restrict str, size_t n, const char *restrict fmt, float fp)
{
	return snprintf(str, n, fmt, fp);
}

int strfromd(char *restrict str, size_t n, const char *restrict fmt, double fp)
{
	return snprintf(str, n, fmt, fp);
}


#ifdef _LIBC
#undef strfromf32
#undef strfromf32x
#undef strfromf64
weak_alias (strfromf, strfromf32)
weak_alias (strfromd, strfromf32x)
weak_alias (strfromd, strfromf64)
#endif
