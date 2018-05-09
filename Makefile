CC=gcc
CFLAGS=
DEPS =
OBJ = main.o parser.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o apush $^ $(CFLAGS)

clean: 
	rm *.o
