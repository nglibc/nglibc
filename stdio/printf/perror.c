#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "rdstate.h"

int     __lockfile  (FILE *f);
int     __unlockfile(FILE *f);

void perror(const char *msg)
{
	FILE *f = stderr;
	char *errstr = strerror(errno);
	int unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	
	/* Save stderr's orientation since perror is not permitted to change */
	int old_mode = rdstate(f) & F_ORIENTED;
	
	if (msg && *msg) {
		fwrite(msg, strlen(msg), 1, f);
		fputc(':', f);
		fputc(' ', f);
	}
	fwrite(errstr, strlen(errstr), 1, f);
	fputc('\n', f);

	rdstate(f) = old_mode | rdstate(f) & ~F_ORIENTED;
	if (unlock) __unlockfile(f);
}


#ifdef _LIBC
libc_hidden_def (perror)
#endif