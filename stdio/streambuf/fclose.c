#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include "streambuf.h"

#ifdef _LIBC
#define _IO_fclose  _IO_compat_fclose
#endif
signed  _IO_fclose(FILE *);

#define stub_alias(old, new) \
extern  __typeof(old) new __attribute__((__weak__, __alias__(#old)))
static void dummy(FILE *f) { }
stub_alias (dummy, __unlist_locked_file);

int __fclose(FILE *f)
{
	extern streambuf __ofl_head;
	int old = (rdstate(f) >> 16 == 0xFBAD);
	if (old) return _IO_fclose(f);

	int unlock = rdstate(f) & F_NEEDLOCK ? __lockfile(f) : 0;
	int r0 = fflush(f);
	int r1 = rdbuf(f)->virt->close(f);
	if (unlock) __unlockfile(f);
	__unlist_locked_file(f);

	/* Past this point, f is closed and any further explict access
	 * to it is undefined. However, it still exists as an entry in
	 * the open file list and possibly in the thread's locked files
	 * list, if it was closed while explicitly locked. Functions
	 * which process these lists must tolerate dead FILE objects
	 * (which necessarily have inactive buffer pointers) without
	 * producing any side effects. */
	if (~rdstate(f) & F_LINKED) {
		streambuf *sb = rdbuf(f);
		int unlock = __lockfile((FILE *)&__ofl_head);
		rdbuf(sb->prev)->next = sb->next;
		rdbuf(sb->next)->prev = sb->prev;
		if (unlock) __unlockfile((FILE *)&__ofl_head);
		//TODO: free(f->getln_buf);
		free(f);
	}
	return r0 | r1;
}

#ifdef _LIBC
#include "libioP.h"
#undef _IO_fclose

int __fcloseall (void)
{
  /* Close all streams.  */
  //TODO: close all files in ofl
  return _IO_cleanup ();
}

weak_alias   (__fcloseall, fcloseall)
strong_alias (__fclose, _IO_new_fclose)
versioned_symbol (libc, __fclose, _IO_fclose, GLIBC_2_1);         
versioned_symbol (libc, _IO_new_fclose, fclose, GLIBC_2_1);
#endif
