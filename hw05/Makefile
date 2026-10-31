CC = cc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -I.

.PHONY: all test bonus clean
all: lock

lock: main.o lock.o
	$(CC) $(CFLAGS) -o $@ $^

test_lock: tests/test_lock.o lock.o
	$(CC) $(CFLAGS) -o $@ $^

test_check: tests/test_check.o check.o lock.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c lock.h
lock.o: lock.c lock.h
check.o: check.c check.h lock.h
tests/test_lock.o: tests/test_lock.c lock.h
tests/test_check.o: tests/test_check.c check.h lock.h

test: test_lock test_check
	./test_lock
	./test_check

bonus: test_check
	./test_check

clean:
	rm -f lock test_lock test_check main.o lock.o check.o tests/*.o
