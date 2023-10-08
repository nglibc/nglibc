#ifndef _RDSTATE_H
#define _RDSTATE_H

#define rdstate(f)      (*(unsigned *)(f))
#define F_EOF             0x00010
#define F_ERR             0x00020
#define F_ORIENTED        0x04000
#define F_NEEDLOCK        0x08000
#define F_WIDE            0x10000

#endif
