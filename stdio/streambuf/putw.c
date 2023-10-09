#define _GNU_SOURCE 1
#include <stdio.h>

int putw(int x, FILE *f)
{
	return (int)fwrite(&x, sizeof x, 1, f)-1;
}
