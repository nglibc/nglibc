#include <stdio.h>
#include "streambuf.h"

int (putchar_unlocked)(int c)
{
	return putc_unlocked(c, stdout);
}
