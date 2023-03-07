CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: all clean

all: static shared dynamic

static:
	$(CC) $(CFLAGS) repl.c ../cw1/libwc.a -o repl.o

shared:
	$(CC) $(CFLAGS) repl.c ../cw1/libwc.so -o repl.o

dynamic:
	$(CC) $(CFLAGS) repl.c -o repl_dyn.o -ldl -D DLL -I.

clean:
	rm -f *.o
