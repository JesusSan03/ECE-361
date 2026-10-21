#include "tally.h"
#include <string.h>
#include <stddef.h>

/* Each entry holds a name and its count */
typedef struct {
    char name[TALLY_LEN + 1];
    int count;
} entry_t;

/* The internal tally structure */
struct tally {
    entry_t entries[TALLY_NAMES];
    int n;
    int in_use;
};

/* Static storage for up to four tallies */
static tally_t pool[TALLY_MAX];

/* Create a new empty tally */
tally_t *tally_create(void)
{
    for (int i = 0; i < TALLY_MAX; i++) {
        if (!pool[i].in_use) {
            pool[i].in_use = 1;
            pool[i].n = 0;
            memset(pool[i].entries, 0,
                   sizeof(pool[i].entries));

            return &pool[i];
        }
    }

    return NULL;
}

/* Destroy a tally and make its slot reusable */
void tally_destroy(tally_t *t)
{
    if (t == NULL) {
        return;
    }

    t->n = 0;
    t->in_use = 0;
    memset(t->entries, 0, sizeof(t->entries));
}

/* Add one occurrence of a name */
int tally_add(tally_t *t, const char *name)
{
    if (t == NULL || name == NULL) {
        return -1;
    }

    size_t len = strlen(name);

    if (len == 0 || len > TALLY_LEN) {
        return -1;
    }

    int pos = 0;

    while (pos < t->n &&
           strcmp(t->entries[pos].name, name) < 0) {
        pos++;
    }

    /* Name already exists */
    if (pos < t->n &&
        strcmp(t->entries[pos].name, name) == 0) {
        t->entries[pos].count++;
        return t->entries[pos].count;
    }

    /* Cannot insert another different name */
    if (t->n >= TALLY_NAMES) {
        return -1;
    }

    /* Shift later entries to make room */
    for (int i = t->n; i > pos; i--) {
        t->entries[i] = t->entries[i - 1];
    }

    /* Insert the new name */
    memcpy(t->entries[pos].name, name, len + 1);
    t->entries[pos].count = 1;
    t->n++;

    return 1;
}

/* Return the count of a particular name */
int tally_count(const tally_t *t, const char *name)
{
    if (t == NULL || name == NULL) {
        return 0;
    }

    for (int i = 0; i < t->n; i++) {
        int comparison = strcmp(t->entries[i].name, name);

        if (comparison == 0) {
            return t->entries[i].count;
        }

        if (comparison > 0) {
            break;
        }
    }

    return 0;
}

/* Return the number of different names */
int tally_size(const tally_t *t)
{
    if (t == NULL) {
        return 0;
    }

    return t->n;
}

/* Print names and counts alphabetically */
void tally_print(const tally_t *t, FILE *out)
{
    if (t == NULL || out == NULL) {
        return;
    }

    for (int i = 0; i < t->n; i++) {
        fprintf(out, "%s %d\n",
                t->entries[i].name,
                t->entries[i].count);
    }
}