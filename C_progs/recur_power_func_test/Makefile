CC=gcc
CFLAGS=-ggdb3 -Wall -Werror -pedantic -std=gnu99

test-power: test-power.o power.o
	$(CC) -o $@ $(CFLAGS) $^

test-power.o: test-power.c
	$(CC) -c -o $@ $(CFLAGS) $<

power.o: power.c
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f *.o *.c~ test-power *.o~
