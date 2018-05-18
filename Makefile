CC=gcc
CFLAGS=-I.
DEPS = main.h builtins.h stack.h parser.h interpreter.h operations.h exec.h heap.h
OBJ = main.o parser.o builtins.o interpreter.o stack.o operations.o heap.c
DESTDIR = /usr/local/bin
.PHONY: all clean purge install

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o apush $^ $(CFLAGS)

clean: 
	rm *.o

purge:
	rm apush
	make clean

install:
	install apush $(DESTDIR)/apush
	echo "/usr/local/bin/apush" >> /etc/shells
