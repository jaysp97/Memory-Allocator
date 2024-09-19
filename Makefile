CC = gcc
CFLAGS = -g -c
AR = ar -rc
RANLIB = ranlib

all:: memgrind.o umalloc.a
	$(CC) memgrind.o umalloc.o -o memgrind

umalloc.a: umalloc.o
	$(AR) libumalloc.a umalloc.o
	$(RANLIB) libumalloc.a

umalloc.o: umalloc.h
	$(CC) $(CFLAGS) umalloc.c

memgrind.o:
	$(CC) $(CFLAGS) memgrind.c -L../ -lumalloc
	
clean:
	rm memgrind *.o *.a
