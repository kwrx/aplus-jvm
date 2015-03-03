#ifdef __arm__

#include <jvm/jvm.h>
#include <stdint.h>
#include <string.h>

#include "../../jconfig.h"


jvalue_t native_call(methodinfo_t* method, jnative_t* native, jvalue_t* params, int params_count) {

	jvalue_t* exp = jmalloc(params_count << 1);
	
	char* s = method->signature;
	register int i = 0, j = 0;
	while(s && params_count--) {
		switch(*s) {
			case 'B':
			case 'C':
			case 'F':
			case 'I':
			case 'L':
			case 'S':
			case 'Z':
			case 'V':
				exp[i++].u32 = params[j++].u32;
				break;
			case 'D':
			case 'J':
				exp[i++].u32 = params[j].u32;
				exp[i++].u32 = (uint32_t) (params[j].u64 >> 32);
				j++;
				break;
			case '[': {
					void* a = (void*) params[j++].ptr;

					register int p;
					int32_t length = ((int32_t*) a) [-1];
					int32_t type = ((int32_t*) a) [-2];
					
					switch(type) {
						case T_LONG:
						case T_DOUBLE:
							for(p = 0; p < length; p++) {
								exp[i++].u32 = (uint32_t) ((uint64_t*) a) [p];
								exp[i++].u32 = (uint32_t) (((uint64_t*) a) [p] >> 32);
							}
							break;
						case T_INT:
						case T_FLOAT:
						case T_REFERENCE:
							for(p = 0; p < length; p++)
								exp[i++].u32 = ((uint32_t*) a) [p];
							
							break;
						case T_BYTE:
						case T_CHAR:
						case T_BOOLEAN:	
							for(p = 0; p < length; p++)
								exp[i++].u8 = ((uint8_t*) a) [p];
							
							break;
						case T_SHORT:
							for(p = 0; p < length; p++)
								exp[i++].u16 = ((uint16_t*) a) [p];
							
							break;
						default:
							j_throw(NULL, JEXCEPTION_INVALID_TYPE);
					}	
				}
				break;		
			default:
				j_throw(NULL, JEXCEPTION_LINKAGE);
		}

		s++;
	}


	#define check_and_set(i)											\
		if(c > i)														\
			__asm__ __volatile__ ("mov r" #i ", %[v]" : : [v]"r"(exp[i].u32))


	static int c = 0;
	c = params_count;

	check_and_set(0);
	check_and_set(1);
	check_and_set(2);
	check_and_set(3);


	/* TODO */

	switch(method->rettype) {
		case T_LONG:
		case T_DOUBLE:
			return (jvalue_t) ((uint64_t (*) ()) native->handler) ();
		default:
			return (jvalue_t) ((uint32_t (*) ()) native->handler) ();
	}
}

#endif
