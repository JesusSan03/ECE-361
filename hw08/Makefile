CC = cc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Werror -pedantic
SANFLAGS = -std=c11 -O1 -g -Wall -Wextra -Werror -pedantic -fsanitize=address,undefined -fno-omit-frame-pointer

.PHONY: all test memcheck bonus clean

all: treestats

treestats: treestats.o bst.o
	$(CC) $(CFLAGS) treestats.o bst.o -o treestats

treestats.o: treestats.c bst.h
	$(CC) $(CFLAGS) -c treestats.c -o treestats.o

bst.o: bst.c bst.h
	$(CC) $(CFLAGS) -c bst.c -o bst.o

tests/test_bst: tests/test_bst.c bst.c bst.h
	$(CC) $(CFLAGS) -I. tests/test_bst.c bst.c -o tests/test_bst

tests/test_remove: tests/test_remove.c bst.c bst.h
	$(CC) $(CFLAGS) -I. tests/test_remove.c bst.c -o tests/test_remove

tests/test_bst_san: tests/test_bst.c bst.c bst.h
	$(CC) $(SANFLAGS) -I. tests/test_bst.c bst.c -o tests/test_bst_san

tests/test_remove_san: tests/test_remove.c bst.c bst.h
	$(CC) $(SANFLAGS) -I. tests/test_remove.c bst.c -o tests/test_remove_san

treestats_san: treestats.c bst.c bst.h
	$(CC) $(SANFLAGS) -I. treestats.c bst.c -o treestats_san

test: treestats tests/test_bst
	./tests/test_bst
	sh tests/test_treestats.sh ./treestats

memcheck: tests/test_bst_san tests/test_remove_san treestats_san
	ASAN_OPTIONS=detect_leaks=0 ./tests/test_bst_san
	ASAN_OPTIONS=detect_leaks=0 ./tests/test_remove_san
	ASAN_OPTIONS=detect_leaks=0 sh tests/test_treestats.sh ./treestats_san

bonus: tests/test_remove_san
	ASAN_OPTIONS=detect_leaks=0 ./tests/test_remove_san

clean:
	rm -f bst.o treestats.o treestats treestats_san
	rm -f tests/test_bst tests/test_remove tests/test_bst_san tests/test_remove_san
