FILES	= main.c interpreter.c expression.c heap.c ../exec.c ../parser.c ../builtins.c
CC	= gcc
CFLAGS	= -I../../include/ \
     -lasan -fsanitize=undefined -lm

test:		$(FILES)
		$(CC) $(CFLAGS) $(FILES) -o test

clean:
	rm test

