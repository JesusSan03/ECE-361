#ifndef ISTACK_H
#define ISTACK_H

#include <stdbool.h>
#include <stddef.h>

#define ISTACK_START 8

typedef struct istack istack_t;

/* Creates an empty stack with capacity ISTACK_START; returns NULL on allocation failure. */
istack_t *istack_create(void);

/* Releases a stack and its values; accepts NULL. Assumes s is not already freed. */
void istack_destroy(istack_t *s);

/* Adds v to the top; returns 0 on success or -1 for NULL or allocation failure. */
int istack_push(istack_t *s, int v);

/* Removes the top value and optionally copies it to *v; returns -1 for NULL or empty. */
int istack_pop(istack_t *s, int *v);

/* Optionally copies the top value to *v without removing it; returns -1 for NULL or empty. */
int istack_peek(const istack_t *s, int *v);

/* Returns the number of held values, or 0 for NULL. */
size_t istack_count(const istack_t *s);

/* Returns true when s is NULL or holds no values. */
bool istack_is_empty(const istack_t *s);

/* Returns the current allocated number of slots, or 0 for NULL. */
size_t istack_capacity(const istack_t *s);

/* Returns a newly allocated bottom-first copy and sets *n; caller frees it.
   Returns NULL and sets *n to 0 for NULL, empty, or allocation failure.
   Assumes n is a valid size_t pointer. */
int *istack_to_array(const istack_t *s, size_t *n);

#endif
