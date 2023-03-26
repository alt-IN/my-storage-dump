CC=gcc
CFLAGS=-ggdb3 -pedantic -Wall -Werror -std=gnu99

.PHONY:
	clean

rotate: rotateMatrix.c
	$(CC) -o $@ $(CFLAGS) $^

clean:
	rm -f rotate *.c~ *.o *.o~
