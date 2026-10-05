CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

all: test_bits

test_bits: bits.o status.o tests/test_bits.o
	$(CC) $(CFLAGS) bits.o status.o tests/test_bits.o -o test_bits

bits.o: bits.c bits.h
	$(CC) $(CFLAGS) -c bits.c

status.o: status.c status.h bits.h
	$(CC) $(CFLAGS) -c status.c

tests/test_bits.o: tests/test_bits.c bits.h status.h
	$(CC) $(CFLAGS) -c tests/test_bits.c -o tests/test_bits.o

test: test_bits
	./test_bits

clean:
	rm -f bits.o status.o tests/test_bits.o test_bits
