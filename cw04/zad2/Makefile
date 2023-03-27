CC = gcc -Wall

all: clean main test

main:
	$(CC) sigtest.c -o sigtest.o

test:
	./sigtest.o > "report.txt"

clean:
	rm -f sigtest.o