CC = gcc -Wall

all: clean main

main:
	$(CC) catcher.c -o catcher.o
	$(CC) sender.c -o sender.o

clean:
	rm -f catcher.o sender.o