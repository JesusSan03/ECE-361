CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

.PHONY: all test clean

all: events tests/test_tally

tally.o: tally.c tally.h
	$(CC) $(CFLAGS) -c tally.c -o tally.o

events.o: events.c tally.h
	$(CC) $(CFLAGS) -c events.c -o events.o

tests/test_tally.o: tests/test_tally.c tally.h
	$(CC) $(CFLAGS) -c tests/test_tally.c -o tests/test_tally.o

events: events.o tally.o
	$(CC) $(CFLAGS) events.o tally.o -o events

tests/test_tally: tests/test_tally.o tally.o
	$(CC) $(CFLAGS) tests/test_tally.o tally.o -o tests/test_tally

test: tests/test_tally
	./tests/test_tally

clean:
	rm -f tally.o events.o tests/test_tally.o events tests/test_tally