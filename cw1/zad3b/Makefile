PROGRAM_STATIC = ../zad2/repl_stat.o
PROGRAM_STATICO = ../zad2/repl_statO.o
PROGRAM_STATICO1 = ../zad2/repl_statO1.o
PROGRAM_STATICO2 = ../zad2/repl_statO2.o
PROGRAM_STATICO3 = ../zad2/repl_statO3.o
PROGRAM_STATICOs = ../zad2/repl_statOs.o

PROGRAM_SHARED = ../zad2/repl_shared.o 
PROGRAM_SHAREDO = ../zad2/repl_sharedO.o 
PROGRAM_SHAREDO1 = ../zad2/repl_sharedO1.o 
PROGRAM_SHAREDO2 = ../zad2/repl_sharedO2.o 
PROGRAM_SHAREDO3 = ../zad2/repl_sharedO3.o 
PROGRAM_SHAREDOs = ../zad2/repl_sharedOs.o 

PROGRAM_DYNAMIC = ../zad2/repl_dyn.o
PROGRAM_DYNAMICO = ../zad2/repl_dynO.o
PROGRAM_DYNAMICO1 = ../zad2/repl_dynO1.o
PROGRAM_DYNAMICO2 = ../zad2/repl_dynO2.o
PROGRAM_DYNAMICO3 = ../zad2/repl_dynO3.o
PROGRAM_DYNAMICOs = ../zad2/repl_dynOs.o

STATIC_OUT = result_static.txt
SHARED_OUT = result_sharec.txt
DYNAMIC_OUT = result_dynamic.txt

REPORT_OUT = report.txt

INPUT = input.txt

.PHONY: all clean

all: test_static test_shared test_dynamic concat_file

test_static:
	echo "staticO0:" > $(STATIC_OUT)
	cat $(INPUT) | $(PROGRAM_STATIC) >> $(STATIC_OUT)
	echo "\nstaticO:" >> $(STATIC_OUT)
	cat $(INPUT) | $(PROGRAM_STATICO) >> $(STATIC_OUT)
	echo "\nstaticO1:" >> $(STATIC_OUT)
	cat $(INPUT) | $(PROGRAM_STATICO1) >> $(STATIC_OUT)
	echo "\nstaticO2:" >> $(STATIC_OUT)
	cat $(INPUT) | $(PROGRAM_STATICO2) >> $(STATIC_OUT)
	echo "\nstaticO3:" >> $(STATIC_OUT)
	cat $(INPUT) | $(PROGRAM_STATICO3) >> $(STATIC_OUT)
	echo "\nstaticOs:" >> $(STATIC_OUT)
	cat $(INPUT) | $(PROGRAM_STATICOs) >> $(STATIC_OUT)

test_shared:
	echo "sharedO0:" > $(SHARED_OUT)
	cat $(INPUT) | $(PROGRAM_SHARED) >> $(SHARED_OUT)
	echo "\nsharedO:" >> $(SHARED_OUT)
	cat $(INPUT) | $(PROGRAM_SHAREDO) >> $(SHARED_OUT)
	echo "\nsharedO1:" >> $(SHARED_OUT)
	cat $(INPUT) | $(PROGRAM_SHAREDO1) >> $(SHARED_OUT)
	echo "\nsharedO2:" >> $(SHARED_OUT)
	cat $(INPUT) | $(PROGRAM_SHAREDO2) >> $(SHARED_OUT)
	echo "\nsharedO3:" >> $(SHARED_OUT)
	cat $(INPUT) | $(PROGRAM_SHAREDO3) >> $(SHARED_OUT)
	echo "\nsharedOs:" >> $(SHARED_OUT)
	cat $(INPUT) | $(PROGRAM_SHAREDOs) >> $(SHARED_OUT)

test_dynamic:
	echo "dynamicO0:" > $(DYNAMIC_OUT)
	cat $(INPUT) | $(PROGRAM_DYNAMIC) >> $(DYNAMIC_OUT)
	echo "\ndynamicO:" >> $(DYNAMIC_OUT)
	cat $(INPUT) | $(PROGRAM_DYNAMICO) >> $(DYNAMIC_OUT)
	echo "\ndynamicO1:" >> $(DYNAMIC_OUT)
	cat $(INPUT) | $(PROGRAM_DYNAMICO1) >> $(DYNAMIC_OUT)
	echo "\ndynamicO2:" >> $(DYNAMIC_OUT)
	cat $(INPUT) | $(PROGRAM_DYNAMICO2) >> $(DYNAMIC_OUT)
	echo "\ndynamicO3:" >> $(DYNAMIC_OUT)
	cat $(INPUT) | $(PROGRAM_DYNAMICO3) >> $(DYNAMIC_OUT)
	echo "\ndynamicOs:" >> $(DYNAMIC_OUT)
	cat $(INPUT) | $(PROGRAM_DYNAMICOs) >> $(DYNAMIC_OUT)

concat_file:
	cat $(STATIC_OUT) $(SHARED_LIB) $(DYNAMIC_OUT) > $(REPORT_OUT)

clean:
	rm -f $(REPORT_OUT) $(STATIC_OUT) $(SHARED_OUT) $(DYNAMIC_OUT)
