CC=gcc
CFLAGS=-I.
DEPS = main.h builtins.h
OBJ = main.o parser.o builtins.o interpreter.o
.PHONY: all clean purge

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o apush $^ $(CFLAGS)

clean: 
	rm *.o

purge:
	rm apush
	make clean
