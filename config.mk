SHELL	:= /bin/bash
PATH	:= /opt/cross/usr/bin:$(PATH)

TOP		:= $(PWD)

ARCH	:= i686
TARGET	:= $(ARCH)-aplus
PREFIX	:= $(TOP)/bin


CC		:= $(TARGET)-gcc
CXX		:= $(TARGET)-g++
ASM		:= nasm
LD		:= $(CC)
OBJCPY	:= $(TARGET)-objcopy

ZIP		:= gzip -f
CP		:= cp
MV		:= mv


DEFINES	:= -D_DEBUG -D_VERBOSE -DTEST -DARCH=\"$(ARCH)\"
FLAGS	:= -fno-strict-aliasing -funroll-all-loops -fno-inline
LIBS	:= -lpthread -lc -lm -lgcc
WARN	:= -Wno-implicit-function-declaration -Wno-unused-result -Wall

CFLAGS	:= $(DEFINES) $(WARN) -I $(TOP)/src/include -c -s -masm=intel -std=c99 -mfpmath=sse -msse3 -Ofast $(FLAGS)
CXXFLAGS:= $(DEFINES) $(WARN) -I $(TOP)/src/include -c -s -masm=intel
AFLAGS	:= $(DEFINES) -f elf
LFLAGS	:= -s


CROSSLIB:= /opt/cross/usr
