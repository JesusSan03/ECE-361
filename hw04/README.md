# HW04 - Tally Program

For this homework its about making a program that counts how many times different names appear. It keeps the names in alphabetical order and uses a limited amount of memory.

tally.h -> Has the function declarations.
tally.c -> Contains the functions that count and store names.
events.c -> Reads the names from a file.
test_tally.c -> Tests if the functions work correctly.
Makefile -> Compiles and tests the program.

```bash
make
make test
./events < data/data.txt
make clean
```

One problem was getting the Makefile to work because I was in the wrong folder. I also had an error saying that main was missing when compiling the test file. This happened because the test file did not have the complete code in it.

I learned how structures can store names and counts together. I also learned how to use a static pool instead of malloc and how to organize a program into different files.