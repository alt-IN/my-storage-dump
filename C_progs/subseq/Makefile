CC=gcc
CGLAGS=-ggdb3 -Wall -Werror -pedantic -std=gnu99 -fsanitize=address
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))

.PHONY:
	clean depend

test-subseq: $(OBJS)
	gcc -o $@ $(CFLAGS) $(OBJS)

depend:
	makedepend $(SRCS)

clean:
	rm -f test-subseq *.o *.o~ *.c~ *.h~
