PROGRAM_STATIC = ../cw2/repl_stat.o
PROGRAM_SHARED = ../cw2/repl_shared.o 
PROGRAM_DYNAMIC = ../cw2/repl_dyn.o

STATIC_OUT = result_static.txt
SHARED_OUT = result_sharec.txt
DYNAMIC_OUT = result_dynamic.txt

INPUT = "init 5\ncount ../cw2/repl.c\ncount ../cw1/wc.c\ncount ../cw1/wc.h\nshow 0\nshow 1\nshow 2\ndelete 2\ndelete 1\ndelete 0\ndestroy\nexit\n"

.PHONY: all clean

all: test_static test_shared test_dynamic concat_file

test_static:
	echo "static:\n" > $(STATIC_OUT)
	(echo $(INPUT) | $(PROGRAM_STATIC)) >> $(STATIC_OUT)

test_shared:
	echo "shared:\n" > $(SHARED_OUT)
	(echo $(INPUT) | $(PROGRAM_SHARED)) >> $(SHARED_OUT)

test_dynamic:
	echo "dynamic:\n" > $(DYNAMIC_OUT)
	(echo $(INPUT) | $(PROGRAM_DYNAMIC)) >> $(DYNAMIC_OUT)

concat_file:
	cat $(STATIC_OUT) $(SHARED_LIB) $(DYNAMIC_OUT) > report.txt
