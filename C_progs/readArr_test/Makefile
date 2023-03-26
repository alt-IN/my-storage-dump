CC=gcc
CFLAGS=-ggdb3 -Wall -Werror -std=gnu99 -fsanitize=address
SRC=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRC))

.PHONY:
	echo "Set phony target"
	clean

test: $(OBJS)
	echo "Buid final program"
	$(CC) -o $@ $(CFLAGS) $(OBJS)

%.o: %.c
	echo "Build object files"
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	echo "Cleaning"
	rm -f *.c~ *.o *.o~ test
