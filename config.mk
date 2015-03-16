SHELL	:= /bin/bash
PATH	:= /opt/cross/usr/bin:$(PATH)

TOP		:= $(PWD)

ARCH	:= i386
TARGET	:= i686-aplus
PREFIX	:= $(TOP)/bin

PLATFORM:= UNIX


CC		:= $(TARGET)-gcc
CXX		:= $(TARGET)-g++
ASM		:= nasm
LD		:= $(CC)
OBJCPY	:= $(TARGET)-objcopy

ZIP		:= gzip -f
CP		:= cp
MV		:= mv


DEFINES	:= -DTEST -DARCH=\"$(ARCH)\" -D__$(ARCH)__ -DCONFIG_$(PLATFORM)
ifeq ($(BUILD),debug)
DEFINES += -DDEBUG -DVERBOSE
endif

FLAGS	:= -fno-strict-aliasing -funroll-all-loops -fno-inline
LIBS	:= -lc -lm -lgcc
WARN	:= -Wno-implicit-function-declaration -Wno-unused-result -Wno-format -Wall


CFLAGS	:= $(DEFINES) $(WARN) -I $(TOP)/src/include -c -s -std=c99 -Ofast $(FLAGS)
CXXFLAGS:= $(DEFINES) $(WARN) -I $(TOP)/src/include -c -s
AFLAGS	:= $(DEFINES) -f elf
LFLAGS	:= 

ifeq ($(ARCH),i386)
CFLAGS	+= -masm=intel -mfpmath=sse -msse3
endif


CROSSLIB:= /opt/cross/usr
