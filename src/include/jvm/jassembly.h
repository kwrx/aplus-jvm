#ifndef _JASSEMBLY_H
#define _JASSEMBLY_H

#include "jconfig.h"
#include "jclass.h"

typedef struct jassembly {
	const char* filename;
	int fd;
	

	jclass_header_t header;
} jassembly_t;

#endif
