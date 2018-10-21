IDIR=../include
CC=gcc
CFLAGS=-I$(IDIR) -lasan -fsanitize=address,undefined -lm

_DEPS = builtins.h exec.h heap.h interpreter.h operations.h parser.h stack.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

ODIR = obj
_OBJ = main.o exec.o parser.o builtins.o \
    scripting/interpreter.o scripting/heap.o \
    scripting/interpreter.o scripting/expression.o \
    scripting/operations.o
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

apush:
	make all

install: apush
	install apush $(DESTDIR)/apush
	@if type chsh >/dev/null 2>&1; then \
		echo To use apush as your login shell:; \
		grep -q -- "$(DESTDIR)$(bindir)/apush" /etc/shells || echo -e " * add the line '$(DESTDIR)$(bindir)/apush' to the file '/etc/shells':" \
		"\n   echo '$(DESTDIR)$(bindir)/apush' | sudo tee -a /etc/shells > /dev/null"; \
		echo " * run 'chsh -s $(DESTDIR)$(bindir)/apush'"; \
		echo; \
	fi;
