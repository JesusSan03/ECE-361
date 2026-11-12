#include "aux.h"
#include <stdlib.h>
/* head is the slot where the NEXT reading will be written. */
struct aux { reading_t items[AUX_CAPACITY]; size_t head; size_t count; };
aux_t *aux_create(void) { return calloc(1, sizeof(aux_t)); }
int aux_add(aux_t *buf, const reading_t *value) {
    if (!buf || !value) return -1;
    buf->items[buf->head] = *value;
    buf->head = (buf->head + 1u) % AUX_CAPACITY;
    if (buf->count < AUX_CAPACITY) ++buf->count;
    return 0;
}
int aux_get_newest(const aux_t *buf, size_t i, reading_t *out) {
    if (!buf || !out || i >= buf->count) return -1;
    size_t slot = (buf->head + AUX_CAPACITY - 1u - i) % AUX_CAPACITY;
    *out = buf->items[slot];
    return 0;
}
size_t aux_count(const aux_t *buf) { return buf ? buf->count : 0; }
int aux_is_full(const aux_t *buf) { return buf && buf->count == AUX_CAPACITY; }
void aux_destroy(aux_t *buf) { free(buf); }
