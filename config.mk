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


DEFINES	:= -DDEBUG -DTEST -DARCH=\"$(ARCH)\"
LIBS	:= -lpthread -lc -lm -lgcc
WARN	:= -Wno-implicit-function-declaration -Wno-unused-result -Wall

CFLAGS	:= $(DEFINES) $(WARN) -I $(TOP)/src/include -c -masm=intel -nostdlib -std=c99 -mfpmath=sse -msse2 -O3 -g -fno-strict-aliasing
CXXFLAGS:= $(DEFINES) $(WARN) -I $(TOP)/src/include -c -masm=intel -nostdlib
AFLAGS	:= $(DEFINES) -f elf
LFLAGS	:=


CROSSLIB:= /opt/cross/usr
