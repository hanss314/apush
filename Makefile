CC=gcc
CFLAGS=
DEPS =
OBJ = main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: main.o
	$(CC) -o apush main.o $(CFLAGS)

clean: 
	rm *.o
