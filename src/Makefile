IDIR=../include
CC=gcc
CFLAGS=-I$(IDIR)

_DEPS = builtins.h exec.h heap.h interpreter.h operations.h parser.h stack.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

ODIR = obj
_OBJ = main.o parser.o builtins.o interpreter.o stack.o operations.o heap.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

DESTDIR = /usr/local/bin
.PHONY: all clean purge install

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o apush $^ $(CFLAGS)

clean: 
	rm -f $(ODIR)/*.o

purge:
	rm apush
	make clean

install:
	install apush $(DESTDIR)/apush
	echo "/usr/local/bin/apush" >> /etc/shells
