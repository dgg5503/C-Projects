#
# Created by gmakemake (Ubuntu Jul 25 2014) on Thu Mar 10 20:09:23 2016
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

CFLAGS = -std=c99 -ggdb -Wall -Wextra -pedantic

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	expNode.c interp.c parser.c stack.c stackNode.c symTbl.c valueMath.c utils.c
PS_FILES =	
S_FILES =	
H_FILES =	expNode.h interp.h parser.h stack.h stackNode.h symTbl.h valueMath.h utils.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	expNode.o parser.o stack.o stackNode.o symTbl.o valueMath.o utils.o

#
# Main targets
#

all:	interp 

interp:	interp.o $(OBJFILES)
	$(CC) $(CFLAGS) -o interp interp.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

expNode.o:	
interp.o:	
parser.o:	
stack.o:	
stackNode.o:	
symTbl.o:	
utils.o:

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) interp.o core

realclean:        clean
	-/bin/rm -f interp 
