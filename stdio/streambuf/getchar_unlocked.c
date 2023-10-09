#include <stdio.h>
#include "streambuf.h"

int (getchar_unlocked)(void)
{
	return getc_unlocked(stdin);
}
