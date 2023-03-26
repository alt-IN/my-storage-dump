CC=gcc
CFLAGS=-ggdb3 -Wall -Werror -pedantic -std=gnu99

.PHONY:
	clean

breaker: breaker.c
	$(CC) -o $@ $(CFLAGS) $<

clean:
	rm -f *.c~ .o~ .o breaker
