PROGRAM_STATIC = ../zad2/repl_stat.o
PROGRAM_SHARED = ../zad2/repl_shared.o 
PROGRAM_DYNAMIC = ../zad2/repl_dyn.o

STATIC_OUT = result_static.txt
SHARED_OUT = result_shared.txt
DYNAMIC_OUT = result_dynamic.txt

REPORT_OUT = report.txt

INPUT = input.txt

.PHONY: all clean

all: test_static test_shared test_dynamic concat_file

test_static:
	echo "static:" > $(STATIC_OUT)
	cat $(INPUT) | $(PROGRAM_STATIC) >> $(STATIC_OUT)

test_shared:
	echo "shared:" > $(SHARED_OUT)
	cat $(INPUT) | $(PROGRAM_SHARED) >> $(SHARED_OUT)

test_dynamic:
	echo "dynamic:" > $(DYNAMIC_OUT)
	cat $(INPUT) | $(PROGRAM_DYNAMIC) >> $(DYNAMIC_OUT)

concat_file:
	cat $(STATIC_OUT) $(SHARED_LIB) $(DYNAMIC_OUT) > $(REPORT_OUT)

clean:
	rm -f $(REPORT_OUT) $(STATIC_OUT) $(SHARED_OUT) $(DYNAMIC_OUT)
