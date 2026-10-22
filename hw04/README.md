# HW04 - Tally Program

For this homework its about making a program that counts how many times different names appear. It keeps the names in alphabetical order and uses a limited amount of memory.

tally.h -> Has the function declarations.
tally.c -> Contains the functions that count and store names.
events.c -> Reads the names from a file.
test_tally.c -> Tests if the functions work correctly.
Makefile -> Compiles and tests the program.

Part 3 - Opaque Type

For this part I tried accessing `t->n` in events.c, which only includes tally.h.

### Compiler Error

```text
events.c:13:17: error: incomplete definition of type 'tally_t' (aka 'struct tally')
   13 |     int test = t->n;
      |                ~^
./tally.h:10:16: note: forward declaration of 'struct tally'
   10 | typedef struct tally tally_t;
      |                ^
1 error generated.
make: *** [events.o] Error 1
```

### Explanation

The error happened because tally.h doesn't show what is inside the tally structure. Only tally.c has the full structure, so events.c cannot access variables like n directly. This is useful because it makes other files use the tally functions instead of changing the data themselves. It also helps prevent mistakes and keeps the code more organized.

```bash
make
make test
./events < data/data.txt
make clean
```

One problem was getting the Makefile to work because I was in the wrong folder. I also had an error saying that main was missing when compiling the test file. This happened because the test file did not have the complete code in it.

I learned how structures can store names and counts together. I also learned how to use a static pool instead of malloc and how to organize a program into different files.