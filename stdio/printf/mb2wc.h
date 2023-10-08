#include <stdlib.h>
#include <wchar.h>
#include <errno.h>

#define mbtowc   mb2wc
#define btowc(x) (x)  

/* Requires src be null terminated string or otherwise at least 4 bytes long */
static int mb2wc(wchar_t *restrict wc, const char *restrict src)
{
	const unsigned char *s = (const void *)src;
	const unsigned UTF_2B  = 0x800, surrog = 0xD800, UTF_3B = 0x10000;
	const unsigned SA = 0xC2, SB = 0xF4, TA = 0x80, TB = 0xC0;
	unsigned c, b;
	wchar_t dummy;

	if (!s) return 0; else c = *s++;
	if (!wc) wc = &dummy;
	if (c < TA) return !!(*wc = c); else c -= SA;
	if (c >= SB-SA) goto invalid_utf8;
	c = c << 6 | (b = *s++ - TA);
	if (b >= TB-TA) goto invalid_utf8;
	if (c < UTF_2B) return (void)(*wc = c), 2;
	c =(c - UTF_2B) << 6;
	if (c < UTF_2B || c-surrog < 0x800 || c-UTF_3B < 0x400 || c >= 0x14400) goto invalid_utf8;
	c = c | (b = *s++ - TA); 
	if (b >= TB-TA) goto invalid_utf8;
	if (c < UTF_3B) return (void)(*wc = c), 3;
	c =(c - UTF_3B) << 6 | (b = *s++ - TA);
	if (b <  TB-TA) return (void)(*wc = c), 4;

invalid_utf8:
	errno = EILSEQ;
	return -1;
}
