
#ifndef HISTOGRAM_H
#define HISTOGRAM_H

/*
 * Prints a temperature histogram to standard output.
 *
 * Uses 20 bins, each 5 degrees Celsius wide.
 * Temperatures outside 0..100 C go into
 * the first or last bin.
 *
 * Assumes n >= 0.
 * Does not modify the input array.
 */
void histogram_print(const float temps[], int n);

#endif
