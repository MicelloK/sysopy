CC = gcc
CFALGS = -Wall
PROGRAM_FORK = "sigtest_fork.o"
PROGRAM_EXECL = "sigtest_execl.o"
REPORT_OUT = "report2.txt"

all: clean main test

main:
	$(CC) $(CFLAGS) sigtest_fork.c -o sigtest_fork.o 
	$(CC) $(CFLAGS) sigtest_execl.c -o sigtest_execl.o

test:
	echo ">>> FORK <<<" > $(REPORT_OUT)
	echo "HANDLER:" >> $(REPORT_OUT)
	./$(PROGRAM_FORK) handler >> $(REPORT_OUT)
	echo "IGNORE:" >> $(REPORT_OUT)
	./$(PROGRAM_FORK) ignore >> $(REPORT_OUT)
	echo "MASK:" >> $(REPORT_OUT)
	./$(PROGRAM_FORK) mask >> $(REPORT_OUT)
	echo "PENDING:" >> $(REPORT_OUT)
	./$(PROGRAM_FORK) pending >> $(REPORT_OUT)

	echo "\n>>> EXECL <<<" >> $(REPORT_OUT)
	echo "IGNORE:" >> $(REPORT_OUT)
	./$(PROGRAM_EXECL) ignore >> $(REPORT_OUT)
	echo "MASK:" >> $(REPORT_OUT)
	./$(PROGRAM_EXECL) mask >> $(REPORT_OUT)
	echo "PENDING:" >> $(REPORT_OUT)
	./$(PROGRAM_EXECL) pending >> $(REPORT_OUT)

clean:
	rm -f $(PROGRAM_FORK) $(PROGRAM_EXECL) $(REPORT_OUT)