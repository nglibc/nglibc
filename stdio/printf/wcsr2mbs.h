#include <wchar.h>
#include <errno.h>

/* Up to 3 bytes of s may be left unutilised, and return value reflects this */
static size_t wcsr2mbs(char *restrict s, const wchar_t **restrict ws, size_t n)
{
	size_t N = n;
	while (n >= 4) {
		wchar_t wc = **ws;
		if (wc-1u < 0x7Fu) {
			*s++ = wc;
			n--;
		} else {
			int sh;
			if (!wc) {
				*s = 0;
				*ws = 0;
				return N-n;
			}
			if (wc < 0x800u) sh = 8; else {
				if (wc < 0xD800u || wc-0xE000u < 0x2000u) sh = 12;
				else if (wc-0x10000u < 0x100000) *s++ = wc>>(sh=18) | 0xF0;
				else return (void)(errno = EILSEQ), -1;
				*s++ = wc>>12 & 0x3F | (26-sh)<<4; // 26-sh is either 0xE or 0x8
			}
			*s++ = wc>>6 & 0x3F | 0x80 | (sh==8)<<6;
			*s++ = wc>>0 & 0x3F | 0x80;
			n -= sh / 4; //Dividing sh by 4 is faster than dividing by 6 & adding 1
		}
		(*ws)++;
	}
	return N-n;
}
