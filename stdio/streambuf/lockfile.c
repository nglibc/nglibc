#include <stdio.h>

int __lockfile(FILE *f)
{
	return flockfile(f), 1;
}

void __unlockfile(FILE *f) {
	return funlockfile(f);
}
