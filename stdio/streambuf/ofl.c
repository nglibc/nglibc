#include <stdio.h>
#include "streambuf.h"

streambuf __ofl_head = {
	.prev  = (void *)&__ofl_head,
	.next  = (void *)&__ofl_head,
	.lock  = (void *)&__ofl_head.lock_data,
};

