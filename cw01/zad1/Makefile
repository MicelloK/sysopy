CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: all clean

all: libwc.a libwc.so

libwc.a: wc.c
	$(CC) $(CFLAGS) -c wc.c
	ar rcs libwc.a wc.o
	rm -f wc.o

libwc.so: wc.c
	$(CC) $(CFLAGS) -fPIC -shared wc.c -o libwc.so

clean:
	rm -f libwc.a libwc.so
	