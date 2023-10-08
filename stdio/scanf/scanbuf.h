#ifndef _SCANBUF_H
#define _SCANBUF_H

#define rdstate(f)      (*(unsigned *)(f))
#define rdbuf(f)        ((streambuf *)(f))
#define F_EOF             0x00010
#define F_ERR             0x00020
#define F_ORIENTED        0x04000
#define F_NEEDLOCK        0x08000
#define F_WIDE            0x10000

typedef struct _IO_cookie_io_functions_t jumptable_t;
typedef struct _IO_FILE FILE;

typedef struct {
	unsigned            flags;
	char               *gptr,  *egptr, *eback;
	char               *pbase, *pptr,  *epptr, *buf, *bufend;
	jumptable_t        *virt;
	void               *cookie;
	FILE               *next_locked, *prev_locked, *next, *prev;
	int                 unused[3];
	void               *lock;
} streambuf;

typedef struct {
	streambuf          *src;
	char               *shend;
	long long           shlim, shcnt;
} scanbuf;

/* Scan helper "stdio" functions for use by scanf-family and strto*-family
 * functions. These accept either a valid stdio FILE, or a minimal pseudo
 * FILE whose buffer pointers point into a null-terminated string. In the
 * latter case, the shfromstr macro should be used to setup the FILE;
 * the rest of the structure can be left uninitialized.
 *
 * To begin using these functions, shlim must first be called on the FILE
 * to set a field width limit, or 0 for no limit. For string pseudo-FILEs,
 * a nonzero limit is not valid and produces undefined behavior. After that,
 * shgetc, shunget, and shcnt are valid as long as no other stdio functions
 * are called on the stream.
 *
 * When used with a real FILE object, shunget has only one byte of pushback
 * available. Further shunget (up to a limit of the stdio UNGET buffer size)
 * will adjust the position but will not restore the data to be read again.
 * This functionality is needed for the wcsto*-family functions, where it's
 * okay because the FILE will be discarded immediately anyway. When used
 * with string pseudo-FILEs, shunget has unlimited pushback, back to the
 * beginning of the string. */

#define shlim(f, lim)   __shlim((f), (lim))
#define shfromstr(f, s) ((f)->src->gptr  = (f)->src->buf = (void *)(s), (f)->src->egptr = (void*)-1)
#define shcnt(f)        ((f)->src->gptr  - (f)->src->buf + (f)->shcnt)
#define shgetc(f)       ((f)->src->gptr != (f)->shend ? (unsigned char)*(f)->src->gptr++ : __shgetc(f))
#define shunget(f)      ((f)->shlim >=0  ? (void)(f)->src->gptr-- : (void)0)

//TODO: add hidden attributes
void                    __shlim(scanbuf *, long long);
int                     __shgetc(scanbuf *);
unsigned long long      __intscan(scanbuf *, unsigned, int, unsigned long long);
long double             __floatscan(scanbuf *, int, int);

#endif
