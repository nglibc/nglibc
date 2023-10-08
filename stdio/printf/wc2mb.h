/* Self contained static wcrtomb function, compact enough to duplicate inline, s cannot be NULL */
#include <stddef.h>
#include <wchar.h>
#include <errno.h>

#define wctomb wc2mb

static size_t wc2mb(char *restrict s, wchar_t wc)
{
	if ((unsigned)wc >= 0x80) {
		int sh;
		if ((unsigned)wc < 0x800) sh = 8; else {
			if ((unsigned)wc < 0xD800 || (unsigned)wc-0xe000 < 0x2000) sh=12;
			else if ((unsigned)wc-0x10000 < 0x100000) *s++ = wc>>(sh=18) | 0xF0;
			else return (void)(errno = EILSEQ), -1;
			*s++ = wc >> 12 & 0x3F | (26-sh) << 4; // (26 - sh) is either 0xE or 0x8
		}
		*s++ = wc >> 6 & 0x3F | 0x80 | (sh==8)<<6;
		*s   = wc >> 0 & 0x3F | 0x80;
		return sh / 4;
	}
	*s = wc;
	return 1;
}
