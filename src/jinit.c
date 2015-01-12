#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>
#include <sys/times.h>

#define _GNU_SOURCE
#include <float.h>
#include <math.h>


#include <jvm/jvm.h>
#include "jconfig.h"

#define THIS	jobject_t* __this


#ifdef DEBUG
#include <execinfo.h>
#include <signal.h>

void sighandler(int sig) {
	void* array[10];
	size_t sz = backtrace(array, 10);

	backtrace_symbols_fd(array, sz, STDERR_FILENO);
	exit(1);
}
#endif



static int __check_super(jassembly_t* child, jassembly_t* parent) {
	jcheck(child && parent);

	if(child->super == NULL)
		return -1;

	jassembly_t* tmp = child->super;
	while(tmp) {
		if(strcmp(tmp->name, parent->name) == 0)		
			return 0;		
		

		tmp = tmp->super;
	}

	return -1;
}







void JNATIVE(registerNatives, V) (void) {
	return;
}

jobject_t* JNATIVE(getClass, L) (THIS) {
	return __this;
}

int JNATIVE(hashCode, L) (THIS) {
	return __this->id;
}

jobject_t* JNATIVE(clone, L) (THIS) {
	return jobject_clone(__this);
}

void JNATIVE(notify, L) (THIS) {
	return;
}

void JNATIVE(notifyAll, L) (THIS) {
	return;
}

void JNATIVE(wait, L) (THIS) {
	return;
}


char* JNATIVE(intern, L) (THIS) {
	return ((jvalue_t) jobject_get_fieldvalue(__this, "intern", T_REFERENCE)).ptr; 
}


jobject_t* JNATIVE(fillInStackTrace, L) (THIS) {
	return __this;
}

int JNATIVE(getStackTraceDepth, L) (THIS) {
	return 0;
}

jobject_t* JNATIVE(getStackTraceElement, LI) (THIS, int index) {
	return NULL;
}



jobject_t* JNATIVE(forName0, LZLL) (char* classname, uint8_t init, jcontext_t* classloader, jassembly_t* caller) {
	if(unlikely(!caller && !classname))
		j_throw(jnative_get_context(), JEXCEPTION_NULL_POINTER);

	jassembly_t* j = (jassembly_t*) jassembly_find(caller, classname);
	if(unlikely(!j))
		return NULL;

	return (jobject_t*) jobject_new_from_assembly(caller, j);
}

uint8_t JNATIVE(isInstance, LL) (THIS, jobject_t* obj) {
	if(unlikely(obj == JCODE_NULL_REFERENCE))
		j_throw(jnative_get_context(), JEXCEPTION_NULL_POINTER);

	return ((strcmp(__this->assembly->name, obj->assembly->name) == 0) || 
			(__check_super(obj->assembly, __this->assembly) == 0));
}

uint8_t JNATIVE(isAssignableFrom, LL) (THIS, jassembly_t* child) {
	if(unlikely(child == JCODE_NULL_REFERENCE))
		j_throw(jnative_get_context(), JEXCEPTION_NULL_POINTER);

	return ((strcmp(__this->assembly->name, child->name) == 0) || 
			(__check_super(child, __this->assembly) == 0));
}

uint8_t JNATIVE(isInterface, L) (THIS) {
	return 0;
}

uint8_t JNATIVE(isArray, L) (THIS) {
	if(JARRAY_MAGIC(__this) == JARRAY_MAGICVALUE)
		return 1;

	return 0;
}

uint8_t JNATIVE(isPrimitive, L) (THIS) {
	#define C(x)									\
		if(strcmp(__this->assembly->name, x) == 0)	\
			return 1;

	C("java/lang/Boolean");
	C("java/lang/Character");
	C("java/lang/Byte");
	C("java/lang/Short");
	C("java/lang/Integer");
	C("java/lang/Long");
	C("java/lang/Float");
	C("java/lang/Double");
	C("java/lang/Void");

	#undef C

	return 0;
}

char* JNATIVE(getName0, L) (THIS) {
	return (char*) strdup(__this->assembly->name);
}

jcontext_t* JNATIVE(getClassLoader0, L) (THIS) {
	return (jcontext_t*) jnative_get_context();
}

jobject_t* JNATIVE(getSuperclass, L) (THIS) {
	return (jobject_t*) jobject_new_from_assembly(__this->assembly, __this->assembly->super);
}

void** JNATIVE(getInterfaces, L) (THIS) {
	return jarray_from_ptr(
		__this->assembly->header.jc_interfaces, 
		sizeof(int16_t), 
		__this->assembly->header.jc_interfaces_count
	);
}

jobject_t* JNATIVE(getComponentType, L) (THIS) {
	return NULL;
}

int JNATIVE(getModifiers, L) (THIS) {
	return __this->assembly->header.jc_access;
}

jobject_t* JNATIVE(getSigners, L) (THIS) {
	return ((jassembly_t*) __this)->signers;
}

void JNATIVE(setSigners, LL) (THIS, jobject_t* signers) {
	((jassembly_t*) __this)->signers = signers;
}

void* JNATIVE(getEnclosingMethod0, L) (THIS) {
	return NULL;
}

jobject_t* JNATIVE(getDeclaringClass0, L) (THIS) {
	return __this;
}

void* JNATIVE(getProtectionDomain0, L) (THIS) {
	return ((jassembly_t*) __this)->protection_domain;
}

void JNATIVE(setProtectionDomain0, LL) (THIS, void* p) {
	((jassembly_t*) __this)->protection_domain = p;
}

jobject_t* JNATIVE(getPrimitiveClass, L) (char* name) {
	jcontext_t* j = (jcontext_t*) jnative_get_context();
	jcheck(j);

	jassembly_t* jx = (jassembly_t*) jassembly_find(j->current_assembly, name);
	if(unlikely(!jx))
		return NULL;

	return (jobject_t*) jobject_new_from_assembly(j->current_assembly, jx);
}

char* JNATIVE(getGenericSignature, L) (THIS) {
	return (char*) __this->assembly->name;
}

uint8_t* JNATIVE(getRawAnnotations, L) (THIS) {
	return NULL;
}

jobject_t* JNATIVE(getConstantPool, L) (THIS) {
	return NULL;
}

void** JNATIVE(getDeclaredFields0, LZ) (THIS, uint8_t publiconly) {
	if(!publiconly)
		return jarray_from_list(__this->assembly->header.jc_fields);

	list_t* t;
	list_init(t);

	list_foreach(v, __this->assembly->header.jc_fields)
		if(((fieldinfo_t*) v)->access & ACC_PUBLIC)
			list_add(t, v);

	return jarray_from_list(t);
}

void** JNATIVE(getDeclaredMethods0, LZ) (THIS, uint8_t publiconly) {
	if(!publiconly)
		return jarray_from_list(__this->assembly->header.jc_fields);

	list_t* t;
	list_init(t);

	list_foreach(v, __this->assembly->header.jc_fields)
		if(((methodinfo_t*) v)->access & ACC_PUBLIC)
			list_add(t, v);

	return jarray_from_list(t);
}

void** JNATIVE(getDeclaredConstructors0, LZ) (THIS, uint8_t publiconly) {
	list_t* t;
	list_init(t);

	list_foreach(v, __this->assembly->header.jc_fields) {
		if(publiconly)
			if((((methodinfo_t*) v)->access & ACC_PUBLIC) == 0)
				continue;

		if(strcmp(((methodinfo_t*) v)->name, "<init>") == 0)
			list_add(t, v);
	}

	return jarray_from_list(t);
}

void** JNATIVE(getDeclaredClasses0, L) (THIS) {
	return jarray_from_list(__this->assembly->deps);
}

uint8_t JNATIVE(desiredAssertionStatus0, L) (jobject_t* obj) {
	return 1;
}




void JNATIVE(initNative, L) (THIS) {
	return;
}

uint8_t JNATIVE(hasStaticInitializer, L) (jobject_t* obj) {
	return 0;
}

void JNATIVE(setIn0, L) (jobject_t* obj) {
	return;
}

void JNATIVE(setOut0, L) (jobject_t* obj) {
	return;
}

void JNATIVE(setErr0, L) (jobject_t* obj) {
	return;
}

long JNATIVE(currentTimeMillis, V) (void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

long JNATIVE(nanoTime, V) (void) {
	double cl = (double) clock();

	return (long) ((cl / CLOCKS_PER_SEC) * 1000000000L);
}

void JNATIVE(arraycopy, LILII) (void* src, int srcpos, void* dst, int dstpos, int count) {
	if(unlikely(!src || !dst))
		j_throw(jnative_get_context(), JEXCEPTION_NULL_POINTER);

	if(unlikely(
		(srcpos < 0)							||
		(dstpos < 0) 							||
		(srcpos + count) > JARRAY_LENGTH(src) 	||
		(dstpos + count) > JARRAY_LENGTH(dst)
	))
		j_throw(jnative_get_context(), JEXCEPTION_ARRAY_BOUNDS);

	if(unlikely(JARRAY_TYPE(src) != JARRAY_TYPE(dst)))
		j_throw(jnative_get_context(), JEXCEPTION_ARRAY_STORE);

	
	register int8_t sz = 1;
	switch(JARRAY_TYPE(src)) {
		case T_BOOLEAN:
		case T_CHAR:
		case T_BYTE:
			sz = sizeof(int8_t);
			break;
		case T_SHORT:
			sz = sizeof(int16_t);
			break;
		case T_INT:
		case T_FLOAT:
		case T_REFERENCE:
			sz = sizeof(int32_t);
			break;
		case T_LONG:
		case T_DOUBLE:
			sz = sizeof(int64_t);
			break;
		default:
			j_throw(jnative_get_context(), JEXCEPTION_INVALID_TYPE);
	}

	memcpy(
		(void*) ((uint32_t) dst + (sz * dstpos)),
		(void*) ((uint32_t) src + (sz * srcpos)),
		(count * sz)
	);
}

int JNATIVE(identityHashCode, L) (jobject_t* obj) {
	if(unlikely(!obj))
		j_throw(jnative_get_context(), JEXCEPTION_NULL_POINTER);

	return obj->id;
}

jobject_t* JNATIVE(initProperties, L) (jobject_t* obj) {
	return obj;
}

char* JNATIVE(mapLibraryName, L) (char* obj) {
	return obj;
}

void* JNATIVE(getClassContext, V) (void) {
	return NULL;
}

jcontext_t* JNATIVE(currentClassLoader0, V) (void) {
	return (jcontext_t*) jnative_get_context();
}

int JNATIVE(classDepth, L) (char* classname) {
	return 0;
}

int JNATIVE(classLoaderDepth0, V) (void) {
	return 0;
}

jobject_t* JNATIVE(currentLoadedClass0, V) (void) {
	jcontext_t* j = (jcontext_t*) jnative_get_context();
	return (jobject_t*) jobject_new_from_assembly(j->current_assembly, j->current_assembly);
}

int jinit() {

#ifdef DEBUG
	signal(SIGSEGV, sighandler);
	signal(SIGABRT, sighandler);
#endif

	#define R(cname, name, desc, ret)	\
		jnative_register_function(cname, #name, #desc, ret, JNATIVE(name, desc))

	
	R("java/lang/Object", registerNatives, V, T_VOID);
	R("java/lang/Object", getClass, L, T_REFERENCE);
	R("java/lang/Object", hashCode, L, T_INT);
	R("java/lang/Object", clone, L, T_REFERENCE);
	R("java/lang/Object", notify, L, T_VOID);
	R("java/lang/Object", notifyAll, L, T_VOID);
	R("java/lang/Object", wait, L, T_VOID);

	R("java/lang/String", intern, L, T_REFERENCE);

	R("java/lang/Throwable", fillInStackTrace, L, T_REFERENCE);
	R("java/lang/Throwable", getStackTraceDepth, L, T_INT);
	R("java/lang/Throwable", getStackTraceElement, LI, T_REFERENCE);

	R("java/lang/Class", registerNatives, V, T_VOID);
	R("java/lang/Class", forName0, LZLL, T_REFERENCE);
	R("java/lang/Class", isInstance, LL, T_BOOLEAN);
	R("java/lang/Class", isAssignableFrom, LL, T_BOOLEAN);
	R("java/lang/Class", isInterface, L, T_BOOLEAN);
	R("java/lang/Class", isArray, L, T_BOOLEAN);
	R("java/lang/Class", isPrimitive, L, T_BOOLEAN);
	R("java/lang/Class", getName0, L, T_REFERENCE);
	R("java/lang/Class", getClassLoader0, L, T_REFERENCE);
	R("java/lang/Class", getSuperclass, L, T_REFERENCE);
	R("java/lang/Class", getInterfaces, L, T_REFERENCE);
	R("java/lang/Class", getComponentType, L, T_REFERENCE);
	R("java/lang/Class", getModifiers, L, T_INT);
	R("java/lang/Class", getSigners, L, T_REFERENCE);
	R("java/lang/Class", setSigners, LL, T_VOID);
	R("java/lang/Class", getEnclosingMethod0, L, T_REFERENCE);
	R("java/lang/Class", getDeclaringClass0, L, T_REFERENCE);
	R("java/lang/Class", getProtectionDomain0, L, T_REFERENCE);
	R("java/lang/Class", setProtectionDomain0, LL, T_VOID);
	R("java/lang/Class", getPrimitiveClass, L, T_REFERENCE);
	R("java/lang/Class", getGenericSignature, L, T_REFERENCE);
	R("java/lang/Class", getRawAnnotations, L, T_REFERENCE);
	R("java/lang/Class", getConstantPool, L, T_REFERENCE);
	R("java/lang/Class", getDeclaredFields0, LZ, T_REFERENCE);
	R("java/lang/Class", getDeclaredMethods0, LZ, T_REFERENCE);
	R("java/lang/Class", getDeclaredConstructors0, LZ, T_REFERENCE);
	R("java/lang/Class", getDeclaredClasses0, L, T_REFERENCE);
	R("java/lang/Class", desiredAssertionStatus0, L, T_BOOLEAN);

	R("java/io/ObjectStreamClass", initNative, L, T_VOID);
	R("java/io/ObjectStreamClass", hasStaticInitializer, L, T_BOOLEAN);

	R("java/lang/System", registerNatives, V, T_VOID);
	R("java/lang/System", setIn0, L, T_VOID);
	R("java/lang/System", setOut0, L, T_VOID);
	R("java/lang/System", setErr0, L, T_VOID);
	R("java/lang/System", currentTimeMillis, V, T_LONG);
	R("java/lang/System", nanoTime, V, T_LONG);
	R("java/lang/System", arraycopy, LILII, T_VOID);
	R("java/lang/System", identityHashCode, L, T_INT);
	R("java/lang/System", initProperties, L, T_REFERENCE);
	R("java/lang/System", mapLibraryName, L, T_REFERENCE);

	R("java/lang/SecurityManager", getClassContext, V, T_REFERENCE);
	R("java/lang/SecurityManager", currentClassLoader0, V, T_REFERENCE);
	R("java/lang/SecurityManager", classDepth, L, T_INT);
	R("java/lang/SecurityManager", classLoaderDepth0, V, T_INT);
	R("java/lang/SecurityManager", currentLoadedClass0, V, T_REFERENCE);

	#undef R

	return 0;
}
