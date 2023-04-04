CC = gcc -Wall

all: clean main test

main:
	$(CC) int.c -o int

test:
	make main
	./int 0.00002 5 > report.txt
	./int 0.00002 500 >> report.txt
	./int 0.00002 50000 >> report.txt

clean:
	rm -f int report.txt