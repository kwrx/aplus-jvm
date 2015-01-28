#ifdef __arm__

#include <jvm/jvm.h>
#include <stdint.h>
#include <string.h>

#include "../../jconfig.h"


jvalue_t native_call(methodinfo_t* method, jnative_t* native, jvalue_t* params, int params_count) {
	if(params_count > 0)
		return (jvalue_t) 0LL;

	switch(method->rettype) {
		case T_LONG:
		case T_DOUBLE:
			return (jvalue_t) ((uint64_t (*) ()) native->handler) ();
		default:
			return (jvalue_t) ((uint32_t (*) ()) native->handler) ();
	}
}

#endif
