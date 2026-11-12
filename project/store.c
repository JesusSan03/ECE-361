#include "store.h"
#include <stdlib.h>
#include <string.h>
/* WEEK 7 IMPLEMENTATION: a sorted resizable array.
 * In Week 8 ONLY this file needs to change into a binary search tree. */
struct store { record_t *items; size_t count; size_t capacity; };
int record_compare(const record_t *a, const record_t *b) {
    if (!a || !b) return 0;
    return (a->reading.timestamp > b->reading.timestamp) -
           (a->reading.timestamp < b->reading.timestamp);
}
store_t *store_create(void) { return calloc(1, sizeof(store_t)); }
static size_t lower_bound(const store_t *s, time_t key) {
    size_t lo = 0, hi = s->count;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2u;
        if (s->items[mid].reading.timestamp < key) lo = mid + 1u;
        else hi = mid;
    }
    return lo;
}
int store_insert(store_t *s, const record_t *r) {
    if (!s || !r) return -1;
    size_t at = lower_bound(s, r->reading.timestamp);
    if (at < s->count && s->items[at].reading.timestamp == r->reading.timestamp) return -1;
    if (s->count == s->capacity) {
        size_t cap = s->capacity ? s->capacity * 2u : 32u;
        if (cap < s->capacity || cap > SIZE_MAX / sizeof(record_t)) return -1;
        record_t *temp = realloc(s->items, cap * sizeof(record_t));
        if (!temp) return -1;
        s->items = temp; s->capacity = cap;
    }
    memmove(s->items + at + 1u, s->items + at, (s->count - at) * sizeof(record_t));
    s->items[at] = *r; ++s->count;
    return 0;
}
const record_t *store_find(const store_t *s, time_t key) {
    if (!s) return NULL;
    size_t at = lower_bound(s, key);
    return at < s->count && s->items[at].reading.timestamp == key ? s->items + at : NULL;
}
void store_walk(const store_t *s, store_visit_fn cb, void *user) {
    if (!s || !cb) return;
    for (size_t i = 0; i < s->count; ++i) cb(&s->items[i], user);
}
size_t store_count(const store_t *s) { return s ? s->count : 0; }
int store_height(const store_t *s) { (void)s; return -1; /* Week 8 */ }
void store_destroy(store_t *s) { if (s) { free(s->items); free(s); } }
