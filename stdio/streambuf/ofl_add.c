#include <stdio.h>
#include "streambuf.h"

extern streambuf __ofl_head;

FILE *__ofl_add(FILE *f)
{
	__lockfile((FILE *)&__ofl_head);
	rdbuf(f)->next =  __ofl_head.next;
	rdbuf(f)->prev = (FILE *)&__ofl_head;
	rdbuf(__ofl_head.next)->prev = f;
	__ofl_head.next = f;
	__unlockfile((FILE *)&__ofl_head);
	return f;
}
