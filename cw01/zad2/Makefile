CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: all clean

all: static shared dynamic

static:
	$(CC) $(CFLAGS) repl.c ../zad1/libwc.a -O0 -o repl_stat.o
	$(CC) $(CFLAGS) repl.c ../zad1/libwc.a -O -o repl_statO.o
	$(CC) $(CFLAGS) repl.c ../zad1/libwc.a -O1 -o repl_statO1.o
	$(CC) $(CFLAGS) repl.c ../zad1/libwc.a -O2 -o repl_statO2.o
	$(CC) $(CFLAGS) repl.c ../zad1/libwc.a -O3 -o repl_statO3.o
	$(CC) $(CFLAGS) repl.c ../zad1/libwc.a -Os -o repl_statOs.o

shared:
	$(CC) $(CFLAGS) repl.c ../zad1/libwc.so -O0 -o repl_shared.o
	$(CC) $(CFLAGS) repl.c ../zad1/libwc.so -O -o repl_sharedO.o
	$(CC) $(CFLAGS) repl.c ../zad1/libwc.so -O1 -o repl_sharedO1.o
	$(CC) $(CFLAGS) repl.c ../zad1/libwc.so -O2 -o repl_sharedO2.o
	$(CC) $(CFLAGS) repl.c ../zad1/libwc.so -O3 -o repl_sharedO3.o
	$(CC) $(CFLAGS) repl.c ../zad1/libwc.so -Os -o repl_sharedOs.o

dynamic:
	$(CC) $(CFLAGS) repl.c -O0 -o repl_dyn.o -ldl -D DLL -I.
	$(CC) $(CFLAGS) repl.c -O -o repl_dynO.o -ldl -D DLL -I.
	$(CC) $(CFLAGS) repl.c -O1 -o repl_dynO1.o -ldl -D DLL -I.
	$(CC) $(CFLAGS) repl.c -O2 -o repl_dynO2.o -ldl -D DLL -I.
	$(CC) $(CFLAGS) repl.c -O3 -o repl_dynO3.o -ldl -D DLL -I.
	$(CC) $(CFLAGS) repl.c -Os -o repl_dynOs.o -ldl -D DLL -I.

clean:
	rm -f *.o
