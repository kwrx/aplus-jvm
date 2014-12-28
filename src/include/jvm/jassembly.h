#ifndef _JASSEMBLY_H
#define _JASSEMBLY_H

#include "jconfig.h"
#include "jclass.h"

typedef struct jassembly {
	const char* filename;
	int fd;
	

	jclass_header_t header;
} jassembly_t;



#define JEXCEPTION_INVALID_OPCODE					"Invalid Opcode"
#define JEXCEPTION_DIVISION_BY_ZERO					"Division by zero"


#define JCODE_NULL_REFERENCE						NULL

#endif
