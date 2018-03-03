# PIC32 device number
DEVICE		= IBM5100


# Above value of ttydev is for johan, the one below is for simon	

# Name of the project
PROGNAME	= GOBLET

# Compiler and linker flags
CFLAGS		+= -std=c11 -O2
ASFLAGS		+= 
LDFLAGS		+= -lSDL2


# Find all source files automatically
CDEPS 			= $(wildcard */*.c)
CFILES          = $(wildcard *.c) 
ASFILES         = $(wildcard *.S)
SYMSFILES	= $(wildcard *.syms)

# Object file names
OBJCDEPS 		= $(CFILES:.c=.c.o)
OBJFILES        = $(CFILES:.c=.c.o)
OBJFILES        +=$(ASFILES:.S=.S.o)
OBJFILES	+=$(SYMSFILES:.syms=.syms.o)

# Hidden directory for dependency files
DEPDIR = .deps
df = $(DEPDIR)/$(*F)


.PHONY: all clean
.SUFFIXES:

all :
	python images/compileImages.py 
	#Compile the c files
	$(CC) $(CDEPS) $(CFILES) $(LDFLAGS) -o $(PROGNAME)

clean:
	$(RM) $(HEXFILE) $(ELFFILE) $(OBJFILES)
	$(RM) -R $(DEPDIR)
	$(RM) images/*.h



