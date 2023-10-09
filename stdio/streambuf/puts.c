#include <stdio.h>
#include <string.h>
#include "streambuf.h"

int __fputs_unlocked(const char *restrict s, FILE *restrict f);

int puts(const char *s)
{
	int r;
	int unlock = rdstate(stdout) & F_NEEDLOCK ? __lockfile(stdout) : 0;
	r = -(__fputs_unlocked(s, stdout) < 0 || putc_unlocked('\n', stdout) < 0);
	if (unlock) __unlockfile(stdout);
	return r;
}
