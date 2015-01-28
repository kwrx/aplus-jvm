SHELL	:= /bin/bash
PATH	:= /opt/cross/usr/bin:$(PATH)

TOP		:= $(PWD)

ARCH	:= arm
TARGET	:= arm-none-eabi
PREFIX	:= $(TOP)/bin


CC		:= $(TARGET)-gcc
CXX		:= $(TARGET)-g++
ASM		:= nasm
LD		:= $(CC)
OBJCPY	:= $(TARGET)-objcopy

ZIP		:= gzip -f
CP		:= cp
MV		:= mv


DEFINES	:= -D_DEBUG -D_VERBOSE -D_TEST -DARCH=\"$(ARCH)\" -D__$(ARCH)__
FLAGS	:= -fno-strict-aliasing -funroll-all-loops -fno-inline
LIBS	:= -lc -lm -lgcc
WARN	:= -Wno-implicit-function-declaration -Wno-unused-result -Wno-format -Wall


CFLAGS	:= $(DEFINES) $(WARN) -I $(TOP)/src/include -c -s -std=c99 -Ofast $(FLAGS)
CXXFLAGS:= $(DEFINES) $(WARN) -I $(TOP)/src/include -c -s
AFLAGS	:= $(DEFINES) -f elf
LFLAGS	:= 

ifeq ($(ARCH),i686)
CFLAGS	+= -masm=intel -mfpmath=sse -msse3
endif


CROSSLIB:= /opt/cross/usr
