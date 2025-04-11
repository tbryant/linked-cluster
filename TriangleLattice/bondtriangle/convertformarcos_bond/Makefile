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
all: convertformarcos display_mgraph display_msubgraph 
convertformarcos: convertformarcos.o
	$(CC) $(CFLAGS) -o convertformarcos.e convertformarcos.o $(LIBS)
display_mgraph: display_mgraph.o
	$(CC) $(CFLAGS) -o display_mgraph.e display_mgraph.o $(LIBS)
display_msubgraph: display_msubgraph.o
	$(CC) $(CFLAGS) -o display_msubgraph.e display_msubgraph.o $(LIBS)

# Implicit rules
convertformarcos:	../graph.c ../bondgraph.c ../topoutils.c convertutils.c
display_mgraph.o:	display_mgraph.c convertutils.c
display_msubgraph.o:	display_msubgraph.c convertutils.c

