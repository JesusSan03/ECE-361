#ifndef PTR_H
#define PTR_H

/* Exchanges the values pointed to by a and b. Assumes both are valid pointers. */
void swap_float(float *a, float *b);

/* Stores the smallest and largest of n values in min and max. Returns 0 on
   success or -1 if n < 1 or any pointer is NULL; outputs stay unchanged on error. */
int minmax(const float *a, int n, float *min, float *max);

/* Returns a pointer to the first of n values greater than threshold, or NULL
   if a is NULL, n < 1, or no value qualifies. Does not modify the array. */
const float *find_above(const float *a, int n, float threshold);

/* Reverses n array elements in place. Does nothing if a is NULL or n < 2. */
void reverse(float *a, int n);

#endif
