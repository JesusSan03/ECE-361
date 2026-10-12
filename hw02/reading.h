
#ifndef READING_H
#define READING_H

#include <stdio.h>

/*
 * Reads sensor measurements from input.
 * Stores ticks, temperatures, and humidities in the given arrays.
 * Returns the number of valid readings.
 * Assumes max_readings > 0 and arrays have enough space.
 */
int reading_read(FILE *input, int ticks[], float temps[],
                 float hums[], int max_readings, int *skipped);

#endif
