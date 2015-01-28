#ifdef __i386__

#include <jvm/jvm.h>
#include <stdint.h>
#include <string.h>

#include "../../jconfig.h"


jvalue_t native_call(methodinfo_t* method, jnative_t* native, jvalue_t* params, int params_count) {
	char* s = method->signature;
	register int i = 0, p = 0;
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
				__asm__ __volatile__ ("mov dword ptr [esp + edi], esi" : : "D"(i), "S"(params[p++].u32));
				i += 4; 
				break;
			case 'D':
			case 'J':
				__asm__ __volatile__ ("mov dword ptr [esp + edi], esi" : : "D"(i), "S"(params[p++].u32));
				__asm__ __volatile__ ("mov dword ptr [esp + edi + 4], esi" : : "D"(i), "S"((int32_t) (params[p++].u64 >> 32)));
				i += 8;
				break;
			case '[': {
					void* a = (void*) params[p++].ptr;
					register int j;
					int32_t length = ((int32_t*) a) [-1];
					int32_t type = ((int32_t*) a) [-2];
					
					switch(type) {
						case T_LONG:
						case T_DOUBLE:
							for(j = 0; j < length; j++) {
								__asm__ __volatile__ ("mov dword ptr [esp + edi], esi" : : "D"(i),  "S"((uint32_t) (((uint64_t*) a) [j])));
								__asm__ __volatile__ ("mov dword ptr [esp + edi + 4], esi" : : "D"(i), "S"((uint32_t) (((uint64_t*) a) [j] >> 32)));

								i += 8;
							}
							break;
						case T_INT:
						case T_FLOAT:
						case T_REFERENCE:
							for(j = 0; j < length; j++) {
								__asm__ __volatile__ ("mov dword ptr [esp + edi], esi" : : "D"(i), "S"(((uint32_t*) a) [j]));
								i += 4;
							}
							break;
						case T_BYTE:
						case T_CHAR:
						case T_BOOLEAN:	
							for(j = 0; j < length; j++) {
								__asm__ __volatile__ ("mov dword ptr [esp + edi], esi" : : "D"(i), "S"((uint32_t) (((uint8_t*) a) [j])));
								i += 4;
							}
							break;
						case T_SHORT:
							for(j = 0; j < length; j++) {
								__asm__ __volatile__ ("mov dword ptr [esp + edi], esi" : : "D"(i), "S"((uint32_t) (((uint16_t*) a) [j])));
								i += 4;
							}
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



	switch(method->rettype) {
		case T_LONG:
		case T_DOUBLE:
			return (jvalue_t) ((uint64_t (*) ()) native->handler) ();
		default:
			return (jvalue_t) ((uint32_t (*) ()) native->handler) ();
	}

	return (jvalue_t) 0LL;
}

#endif
