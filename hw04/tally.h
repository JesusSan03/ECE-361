#ifndef TALLY_H
#define TALLY_H

#include <stdio.h>

#define TALLY_MAX 4
#define TALLY_NAMES 16
#define TALLY_LEN 15

typedef struct tally tally_t;

/* Creates an empty tally. */
tally_t *tally_create(void);

/* Destroys a tally so its slot can be reused. */
void tally_destroy(tally_t *t);

/* Adds one occurrence of name. */
int tally_add(tally_t *t, const char *name);

/* Returns how many times name was added.*/
int tally_count(const tally_t *t, const char *name);

/* Returns the number of different names.*/
int tally_size(const tally_t *t);

/* Prints names and counts alphabetically.*/
void tally_print(const tally_t *t, FILE *out);

#endif