#ifndef _JASSEMBLY_H
#define _JASSEMBLY_H

#include "jconfig.h"
#include "jclass.h"

typedef struct jassembly {
	const char* filename;
	int fd;
	
	jclass_header_t header;
} jassembly_t;


typedef struct jobject {
	void* ref;
	int refcount;
	int lock;
} jobject_t;



#define JEXCEPTION_INVALID_OPCODE					"Invalid Opcode"
#define JEXCEPTION_DIVISION_BY_ZERO					"Division by zero"
#define JEXCEPTION_NEGATIVE_ARRAY_SIZE				"Negative array size"
#define JEXCEPTION_ARRAY_BOUNDS						"Index out of bounds of array"
#define JEXCEPTION_NULL_POINTER						"Null pointer"
#define JEXCEPTION_NULL_REFERENCE					"Null reference"


#define JCODE_NULL_REFERENCE						NULL

#endif
