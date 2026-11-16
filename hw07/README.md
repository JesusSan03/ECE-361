# Homework 7 - Dynamic Memory

## What This Program Does

This homework has two main parts. The first part is a stack that holds whole numbers. It starts with room for 8 numbers. When it gets full, it makes room for twice as many numbers. The second part is a bracket checker. It checks if parentheses, brackets, and braces match up in a file. It also tells you where the first problem is.

## How to Run It

Open a terminal in the hw07 folder and enter and type:

make

make test

make memcheck

make clean

The make command builds the bracket checker. Make test runs the tests to see if everything works. Make memcheck checks for memory problems. Make clean removes the programs that were built.

To check an example file, type:

make

./balance < inputs/balanced.txt

## Part 4 - Memory Mistakes

For this part, three mistakes were added one at a time to see what would happen. After testing each mistake, it was removed from the code.

### 1. Destroying the Same Stack Twice

First line of the report:

==625==ERROR: AddressSanitizer: heap-use-after-free on address 0x503000000040 at pc 0x557bfc783496 bp 0x7fff144b1d20 sp 0x7fff144b1d18

The stack was destroyed two times. The first time freed the memory. The second time tried to use memory that was already freed. This caused the memory checker to find an error.

### 2. Pushing After Destroying the Stack

First line of the report:

==626==ERROR: AddressSanitizer: heap-use-after-free on address 0x503000000048 at pc 0x56116214254c bp 0x7ffcfca7e6a0 sp 0x7ffcfca7e698

The stack was destroyed, but the program still tried to push another number into it. This caused an error because the memory was already freed and could not be used anymore.

### 3. Not Destroying the Stack

First line of the report:

==627==ERROR: LeakSanitizer: detected memory leaks

For this test, the istack_destroy function was removed from balance.c. The program ended without freeing the memory that was used for the stack. The memory checker found a memory leak of 56 bytes. This showed why it is important to free memory when it is no longer needed.

## Other Things to Know

The stack uses malloc, realloc, and free to manage memory. It uses a temporary pointer with realloc so the old memory is not lost if something goes wrong. The stack can grow bigger, but it does not get smaller.

The bracket checker ignores brackets inside comments and strings. It counts lines and columns starting at 1. It stops when it finds the first bracket mistake.

The tests check things like empty stacks, NULL pointers, adding more than 8 numbers, pushing and popping lots of numbers, and making a copy of the stack into an array.