#ifndef AUX_H
#define AUX_H
/* Opaque circular buffer: the last 16 sensor readings. */
#include "sensor.h"
#include <stddef.h>
#define AUX_CAPACITY 16u
typedef struct aux aux_t;
aux_t *aux_create(void);
int aux_add(aux_t *buf, const reading_t *value);
/* Index 0 means newest, index 1 is one older, etc. */
int aux_get_newest(const aux_t *buf, size_t i, reading_t *out);
size_t aux_count(const aux_t *buf);
int aux_is_full(const aux_t *buf);
void aux_destroy(aux_t *buf);
#endif
