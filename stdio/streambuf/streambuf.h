/* Stream Buffer is accessed from FILE using rdbuf()
   It provides pointers to get & put areas, similar to C++ streambuffer
   It needs #define _GNU_SOURCE 1 to access virtual function jump table fields */
   
#ifndef _STREAMBUF_H
#define _STREAMBUF_H

#define rdstate(f)      (*(unsigned *)(f))
#define rdbuf(f)        ((streambuf *)(f))
#define F_NORD            0x00004
#define F_NOWR            0x00008
#define F_EOF             0x00010
#define F_ERR             0x00020
#define F_LBF             0x00200
#define F_CURWR           0x00800
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
	int                 lock_data[3];
	void               *lock;
} streambuf;

//TODO: add hidden attributes
int     __lockfile  (FILE *);
int     __unlockfile(FILE *);
int     __toread    (FILE *);
int     __towrite   (FILE *);
void    __setg      (FILE *, char*, char*, char*);
FILE   *__finit     (FILE *, jumptable_t *, void *, char *, size_t);
void   *__fcookie   (FILE *);
void    __fsetwrite (FILE *);
void    __fseterror (FILE *);
char   *__pbase     (FILE *);
int     __pflush    (FILE *, const void *);
size_t  __fwritex   (const char *, size_t, FILE *);
int     __uflowx    (FILE *);
int     __overflowx (FILE *, int);
void    __stdio_exit_needed(void);

#endif
