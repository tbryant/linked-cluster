# The following lines are required because standard make does not
# recognize the Objective-C .m suffix.

.SUFFIXES: .o .m
.m.o:
	$(CC) -c $(CFLAGS) $<


# Macros

CC = gcc
CFLAGS = -g -pedantic -Wall -ggdb -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_GNU_SOURCE
LIBS = -lm

# Explicit rule
all: makebondgraph displaybondgraph  maketopolist displaytopolist converttopolist makesubtopolist displaysubtopolist
makebondgraph: makebondgraph.o
	$(CC) $(CFLAGS) -o makebondgraph.e makebondgraph.o $(LIBS)
displaybondgraph: displaybondgraph.o
	$(CC) $(CFLAGS) -o displaybondgraph.e displaybondgraph.o $(LIBS)
maketopolist: maketopolist.o
	$(CC) $(CFLAGS) -o maketopolist.e maketopolist.o $(LIBS)
displaytopolist: displaytopolist.o
	$(CC) $(CFLAGS) -o displaytopolist.e displaytopolist.o $(LIBS)
converttopolist: converttopolist.o
	$(CC) $(CFLAGS) -o converttopolist.e converttopolist.o $(LIBS)
makesubtopolist: makesubtopolist.o
	$(CC) $(CFLAGS) -o makesubtopolist.e makesubtopolist.o $(LIBS)
displaysubtopolist: displaysubtopolist.o
	$(CC) $(CFLAGS) -o displaysubtopolist.e displaysubtopolist.o $(LIBS)

# Implicit rules

makebondgraph.o:	makebondgraph.c graph.c bondgraph.c dlbondgraphbig.c
displaybondgraph.o:	displaybondgraph.c graph.c bondgraph.c
maketopolist.o:	maketopolist.c graph.c bondgraph.c dltopo.c topoutils.c
displaytopolist.o:	displaytopolist.c graph.c bondgraph.c topoutils.c
converttopolist.o:	converttopolist.c graph.c bondgraph.c topoutils.c
makesubtopolist.o:	makesubtopolist.c graph.c bondgraph.c dltopo.c topoutils.c
displaysubtopolist.o:	displaysubtopolist.c graph.c bondgraph.c topoutils.c


