
#ifndef STATS_H
#define STATS_H

/*
 * Returns the minimum value in an array.
 * Assumes n >= 1.
 */
float stats_min(const float a[], int n);

/*
 * Returns the maximum value recursively.
 * Assumes n >= 1.
 */
float stats_max(const float a[], int n);

/*
 * Returns the arithmetic mean of an array.
 * Assumes n >= 1.
 */
float stats_mean(const float a[], int n);

/*
 * Returns the longest consecutive run of values
 * strictly greater than threshold.
 * Writes the starting index through start_index.
 * If no values qualify, sets start_index to -1.
 * Assumes n >= 0 and start_index is not NULL.
 */
int stats_longest_run_above(const float a[], int n,
                            float threshold, int *start_index);

#endif
