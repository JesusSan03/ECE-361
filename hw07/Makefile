CC = cc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -g
SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

.PHONY: all test memcheck clean
all: balance

balance: balance.c istack.c istack.h
	$(CC) $(CFLAGS) balance.c istack.c -o $@

tests/test_istack: tests/test_istack.c istack.c istack.h
	$(CC) $(CFLAGS) tests/test_istack.c istack.c -o $@

test: balance tests/test_istack
	./tests/test_istack
	sh ./regress.sh

# Linux: AddressSanitizer/UndefinedBehaviorSanitizer and LeakSanitizer.
# macOS: Apple leaks tool for leak checks; sanitizer builds for memory errors.
ifeq ($(shell uname -s),Darwin)
memcheck: balance tests/test_istack
	$(CC) $(CFLAGS) $(SANFLAGS) tests/test_istack.c istack.c -o tests/test_istack_san
	$(CC) $(CFLAGS) $(SANFLAGS) balance.c istack.c -o balance_san
	./tests/test_istack_san
	sh ./regress.sh ./balance_san
	leaks --atExit -- ./tests/test_istack
	leaks --atExit -- ./balance < inputs/balanced.txt
else
memcheck:
	$(CC) $(CFLAGS) $(SANFLAGS) tests/test_istack.c istack.c -o tests/test_istack_san
	$(CC) $(CFLAGS) $(SANFLAGS) balance.c istack.c -o balance_san
	ASAN_OPTIONS=detect_leaks=1 ./tests/test_istack_san
	ASAN_OPTIONS=detect_leaks=1 sh ./regress.sh ./balance_san
endif

clean:
	rm -f balance balance_san tests/test_istack tests/test_istack_san
