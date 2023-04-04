CC = gcc -Wall

all: clean main test

main:
	$(CC) int.c -o int
	$(CC) part.c -o part

test:
	make main
	./int 0.000002 5 > report.txt
	./int 0.000002 10 >> report.txt
	./int 0.000002 50 >> report.txt

clean:
	rm -f int part report.txt
