#define strfromf64x __hide_strfromf64x
#define strfromf128 __hide_strfromf128
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>

int strfroml(char *restrict s, size_t n, const char *restrict format, long double fp) {
	size_t len = strlen(format);
	char ff[len+2];
	memcpy(ff, format, len-1);
	ff[len-1] = 'L';
	ff[len] = format[len-1];
	ff[len+1] = 0;
	return snprintf(s, n, ff, fp);
}

#ifdef _LIBC
#undef strfromf64x
#undef strfromf128
#if (LDBL_MANT_DIG == 64 || LDBL_MANT_DIG == 113) && LDBL_MAX_EXP == 16384
weak_alias (strfroml, strfromf64x)
#endif
#if (LDBL_MANT_DIG == 113) && LDBL_MAX_EXP == 16384
weak_alias (strfroml, strfromf128)
#endif
#endif
