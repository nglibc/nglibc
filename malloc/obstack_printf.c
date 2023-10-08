#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "obstack.h"

int obstack_vprintf(struct obstack *obstack, const char *restrict fmt, va_list ap)
{
	char buf[1024];
	int len = vsnprintf(buf, sizeof(buf), fmt, ap);
	obstack_grow(obstack, buf, len);
	return len;
}

int obstack_printf(struct obstack *obstack, const char *restrict fmt, ...)
{
	int len;
	va_list ap;
	va_start(ap, fmt);
	len = obstack_vprintf(obstack, fmt, ap);
	va_end(ap);
	return len;
}
