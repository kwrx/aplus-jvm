OUTPUT	:= bin/avm
RUNTIME	:= bin/rt.jpk
JPK		:= bin/jpk

CC		:= gcc
LD		:= gcc
JCC		:= gcj

FIND	:= /usr/bin/find

WARN	:= -Wall -Wno-unused-result
OPT		:= -Ofast -mfpmath=sse

INCDIR	:= src/include
LIBS	:= -lffi


CFILES	:= $(shell $(FIND) src -type f -name "*.c")
TFILES	:= $(shell $(FIND) test -type f -name "*.java")
JFILES	:= $(shell $(FIND) lib -type f -name "*.java")

OFILES	:= $(CFILES:.c=.o)
TCFILES	:= $(TFILES:.java=.class)
JCFILES	:= $(JFILES:.java=.class)


.SUFFIXES: .java .class

all: $(OUTPUT) $(RUNTIME) lib test
.c.o:
	@echo "  CC   " $<
	@$(CC) -c $< -o $@ $(OPT) $(WARN) -I $(INCDIR) -pipe -fomit-frame-pointer -include config.h
	
$(OUTPUT): $(OFILES)
	@echo "  LD   " $@
	@$(LD) -o $@ $(OFILES) $(LIBS)
	
test: $(TCFILES)
lib: $(JCFILES)
.java.class:
	@echo "  JAVA " $<
	@$(JCC) --classpath=lib -C $<


$(RUNTIME): $(JPK) $(JCFILES)
	@echo "  JPK  " $(RUNTIME)
	@$(JPK) $(RUNTIME) $(JCFILES)
$(JPK): src/jpk.c
	@echo "  CC   " $(JPK)
	@$(CC) -O2 -o $(JPK) src/jpk.c -I $(INCDIR) $(WARN) -D__JPK__ -include config.h
	
	
clean:
	@$(RM) $(OFILES)
	@$(RM) $(JCFILES)
	@$(RM) $(TCFILES)
	@$(RM) $(OUTPUT)
	@$(RM) $(RUNTIME)
	@$(RM) $(JPK)

