#ifndef _CONFIG_H
#define _CONFIG_H


#define DEBUG			0
#define TEST			0
#define FREESTANDING		0

#define CONFIG_JNI		0


#define APP_NAME		"avm"
#define APP_VERSION		"0.1-alpha"
#define APP_COPY		"2015 Antonio Natale"
#define APP_CDATE		__DATE__
#define APP_CTIME		__TIME__

#ifdef __GNUC__
#define APP_CC			"gcc"
#define APP_CC_VERSION		__VERSION__
#else
#define APP_CC			"unknown compiler"
#define APP_CC_VERSION		""
#endif

#define APP_VERSION_FORMAT										\
		"%s %s\nCopyright (C) %s\nBuilt with %s %s (%s:%s)\n"
#define APP_VERSION_ARGS										\
		APP_NAME, APP_VERSION, APP_COPY, APP_CC, APP_CC_VERSION,				\
		APP_CDATE, APP_CTIME


#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>

#if DEBUG && !FREESTANDING
#include <stdlib.h>
#include <stdio.h>
#define LOG(x)			\
	{ printf("%s: %s\n", APP_NAME, x); }
#define LOGF(x, y...)	\
	{ printf("%s: " x "\n", APP_NAME, y); }

#define ASSERT(x)	\
	{ if(unlikely(!(x))) { LOGF("Assertion \"%s\" failed in %s:%d", #x, __FILE__, __LINE__); abort(); } }

#else
#define LOG(x)
#define LOGF(x, y...)
#define ASSERT(x) ((void) x)
#endif

#if FREESTANDING
#define PRINTF(x, y...)
#define EXIT(x)	for(;;)
#else
#include <stdlib.h>
#include <stdio.h>
#define PRINTF(x, y...)	\
	{ printf(x, y); }
#define EXIT(x) exit(x)
#endif


#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wnonnull"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif





#define __SYSROOT__			"/xdev/cross"
#define __PREFIX__			__SYSROOT__ "/prj/AVM/bin"
#define __LIBDIR__			__PREFIX__ "/lib"

#endif
