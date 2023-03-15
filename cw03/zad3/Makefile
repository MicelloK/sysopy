CC = gcc
CFLAGS = -Wall

SOURCE = treeproc.c
PROGRAM = treeproc.o
DIR = ../../..
INPUT = input.txt
REPORT = repor.txt

all: main

main:
	$(CC) $(CFLAGS) $(SOURCE) -o $(PROGRAM)

test:
	echo "DIR: ../../.." > $(REPORT)
	while read -r arg; do \
		echo "SEARCH STRING [$$arg]: " >> $(REPORT); \
		./$(PROGRAM) $(DIR) $$arg >> $(REPORT); \
	done < $(INPUT)

clean:
	rm -f $(PROGRAM)  $(REPORT)
