#ifndef BST_H
#define BST_H
#include <stdbool.h>
#include <stddef.h>

/* The details of the tree are hidden in bst.c. */
typedef struct bst bst_t;

/* Makes an empty tree. Returns NULL if memory allocation fails. */
bst_t *bst_create(void);

/* Frees the tree and all its nodes. A NULL tree is allowed. */
void bst_destroy(bst_t *t);

/* Adds a key. Equal keys go right. Returns 0 if it worked, -1 on error.
 * The tree is unchanged if t is NULL or memory allocation fails. */
int bst_insert(bst_t *t, int key);

/* Looks for a key. Returns true if found, or false if not found.
 * If visits is not NULL, it gets the number of nodes checked.
 * A NULL tree counts as zero visits. */
bool bst_contains(const bst_t *t, int key, size_t *visits);

/* Counts the stored keys, including duplicates. NULL gives zero. */
size_t bst_size(const bst_t *t);

/* Counts nodes on the longest root-to-leaf path. NULL/empty gives zero. */
size_t bst_height(const bst_t *t);

/* Bonus: removes one matching key. Returns 0 on success, -1 if t is
 * NULL or the key is missing. The tree stays the same on failure. */
int bst_remove(bst_t *t, int key);

#endif
