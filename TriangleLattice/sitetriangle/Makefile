# The following lines are required because standard make does not
# recognize the Objective-C .m suffix.

.SUFFIXES: .o .m
.m.o:
	$(CC) -c $(CFLAGS) $<


# Macros

CC = gcc
CFLAGS = -g -pedantic -Wall -ggdb -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_GNU_SOURCE
LIBS = -lm
SRC = globals.h

# Explicit rule
all: count displayM makesitetopolist sortsitetopolist displaysitetopolist makesitesubtopolist displaysitesubtopolist
count: count.o
	$(CC) $(CFLAGS) -o count.e count.o $(LIBS)
displayM: displayM.o
	$(CC) $(CFLAGS) -o displayM.e displayM.o $(LIBS)
makesitetopolist: makesitetopolist.o
	$(CC) $(CFLAGS) -o makesitetopolist.e makesitetopolist.o $(LIBS)
sortsitetopolist: sortsitetopolist.o
	$(CC) $(CFLAGS) -o sortsitetopolist.e sortsitetopolist.o $(LIBS)
displaysitetopolist: displaysitetopolist.o
	$(CC) $(CFLAGS) -o displaysitetopolist.e displaysitetopolist.o $(LIBS)
makesitesubtopolist: makesitesubtopolist.o
	$(CC) $(CFLAGS) -o makesitesubtopolist.e makesitesubtopolist.o $(LIBS)
displaysitesubtopolist: displaysitesubtopolist.o
	$(CC) $(CFLAGS) -o displaysitesubtopolist.e displaysitesubtopolist.o $(LIBS)

# Implicit rules

count.o:	count.c graph.c dlembed.c
displayM.o:	displayM.c graph.c
makesitetopolist.o:	makesitetopolist.c sitesquareutils.c dltopo.c
sortsitetopolist.o:	sortsitetopolist.c sitesquareutils.c 
displaysitetopolist.o:	displaysitetopolist.c sitesquareutils.c 
makesitesubtopolist.o:	makesitesubtopolist.c sitesquareutils.c dltopo.c
displaysitesubtopolist.o:	displaysitesubtopolist.c sitesquareutils.c

# Clean rule
clean:
	rm -f *.o *.e