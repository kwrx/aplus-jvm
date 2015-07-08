OUTPUT	:= bin/avm
OUTLIB	:= bin/lib/libavm.a
RUNTIME	:= bin/lib/rt.jar

CC		:= gcc
LD		:= gcc
JCC		:= gcj
JAR		:= jar -cf
AR		:= ar -rcs

FIND	:= /usr/bin/find

WARN	:= -Wall -Wno-unused-result -Wno-unused-variable
OPT		:= -Ofast -mfpmath=sse -msse2 \
			-funroll-all-loops -faggressive-loop-optimizations \
			-fomit-frame-pointer -fira-loop-pressure -floop-strip-mine \
			-fno-align-functions -fno-align-loops

INCDIR	:= src/include
LIBS	:= -lffi -lzip -lz


CFILES	:= $(shell $(FIND) src -type f -name "*.c")
TFILES	:= $(shell $(FIND) test -type f -name "*.java")
JFILES	:= $(shell $(FIND) lib -type f -name "*.java")

OFILES	:= $(CFILES:.c=.o)
TCFILES	:= $(TFILES:.java=.class)
JCFILES	:= $(JFILES:.java=.class)


.SUFFIXES: .java .class

all: $(OUTPUT) $(OUTLIB) $(RUNTIME) lib test
.c.o:
	@echo "  CC   " $<
	@$(CC) -c $< -o $@ $(OPT) $(WARN) -I $(INCDIR) -pipe -include config.h
	
$(OUTPUT): $(OFILES)
	@echo "  LD   " $@
	@$(LD) -o $@ $(OFILES) $(LIBS)

$(OUTLIB): $(OFILES)
	@echo "  AR   " $@
	@$(AR) $@ $(filter-out src/main.o,$(OFILES))
	
test: $(TCFILES)
lib: $(JCFILES)
.java.class:
	@echo "  JAVA " $<
	@$(JCC) --classpath=lib -C $<


$(RUNTIME): $(JCFILES)
	@echo "  JAR  " $(RUNTIME)
	@cd lib && $(JAR) ../$(RUNTIME) $(subst lib/,,$(JCFILES))
	
	
fastclean:
	@$(RM) $(OFILES)
	@$(RM) $(OUTPUT)

clean:
	@$(RM) $(OFILES)
	@$(RM) $(JCFILES)
	@$(RM) $(TCFILES)
	@$(RM) $(OUTPUT)
	@$(RM) $(OUTLIB)
	@$(RM) $(RUNTIME)

