CC = gcc
CFLAGS = -Wall -Wextra
LIB_NAME = wc
LIB_SRC = wc.c
STATIC_LIB = lib$(LIB_NAME).a
SHARED_LIB = lib$(LIB_NAME).so

.PHONY: all clean

all: $(STATIC_LIB) $(SHARED_LIB)

$(STATIC_LIB): $(LIB_SRC)
	cd ../cw1; make
	cd ../cw2; make
	$(CC) $(CFLAGS) -c wc.c
	ar rcs $@ $(LIB_NAME).o
	rm -f $(LIB_NAME).o

$(SHARED_LIB): $(LIB_SRC)
	$(CC) $(CFLAGS) -fPIC -shared wc.c -o $@

clean:
	rm -f $(STATIC_LIB) $(SHARED_LIB)
	