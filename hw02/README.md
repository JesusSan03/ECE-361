
ECE 361 - Homework 2

Description

For this homework, I took the original readings.c program and separated it into different modules. The program still does the same thing, but the code is now organized into different files. It reads temperature and humidity data, calculates statistics, finds the longest run above a temperature threshold, and prints a histogram.

Modules

main.c --> Runs the program and calls the other modules.

reading.c / reading.h --> Reads the sensor data and skips invalid lines.

stats.c / stats.h --> Calculates the minimum, maximum, mean, and longest run above a threshold.

histogram.c / histogram.h --> Creates the temperature histogram.

tests/test_stats.c --> Tests the statistical functions.

Module Diagram

main.c
  |
  |--> reading.c --> reading.h
  |
  |--> stats.c --> stats.h
  |
  |--> histogram.c --> histogram.h

How to Build

Open the terminal inside the hw02 folder and run:

make

This compiles the program and creates the readings executable.

How to Run

Run the program using the normal data:

./readings < data/normal.txt

Run the program with a different temperature threshold:

./readings 25 < data/edge.txt

How to Test

Run:

make test

This runs the unit tests and regression tests. The unit tests check the statistical functions, including the recursive maximum function. The regression tests make sure the program still gives the same output as the original version.

How to Clean

Run:

make clean

This removes the executable and object files.

Summary

The main goal of this homework was to make the program more organized by separating it into modules. I also removed the global variables, used recursion for the maximum function, and added tests and a Makefile. The program still gives the same results as the original.
