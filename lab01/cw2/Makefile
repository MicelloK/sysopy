CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: all clean

all: static shared dynamic

static:
	$(CC) $(CFLAGS) repl.c ../cw1/libwc.a -O0 -o repl_stat.o
	$(CC) $(CFLAGS) repl.c ../cw1/libwc.a -Os -o repl_statOs.o

shared:
	$(CC) $(CFLAGS) repl.c ../cw1/libwc.so -O0 -o repl_shared.o
	$(CC) $(CFLAGS) repl.c ../cw1/libwc.so -Os -o repl_sharedOs.o

dynamic:
	$(CC) $(CFLAGS) repl.c -O0 -o repl_dyn.o -ldl -D DLL -I.
	$(CC) $(CFLAGS) repl.c -Os -o repl_dynOs.o -ldl -D DLL -I.

clean:
	rm -f *.o
