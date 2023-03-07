CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LIB_NAME = wc
LIB_SRC = wc.c
STATIC_LIB = lib$(LIB_NAME).a
SHARED_LIB = lib$(LIB_NAME).so

.PHONY: all clean

all: $(STATIC_LIB) $(SHARED_LIB)

$(STATIC_LIB): $(LIB_SRC)
	$(CC) $(CFLAGS) -c $^
	ar rcs $@ $(LIB_NAME).o
	rm -f $(LIB_NAME).o

$(SHARED_LIB): $(LIB_SRC)
	$(CC) $(CFLAGS) -fPIC -shared $^ -o $@

clean:
	rm -f $(STATIC_LIB) $(SHARED_LIB)