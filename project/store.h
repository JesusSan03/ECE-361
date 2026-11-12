#ifndef STORE_H
#define STORE_H
/* Public interface does not show the data structure inside the store. */
#include "sensor.h"
#include "fsm.h"
#include <stdint.h>
#include <stddef.h>
typedef struct { reading_t reading; state_t state; uint32_t status; } record_t;
typedef struct store store_t;
typedef void (*store_visit_fn)(const record_t *record, void *user);
int record_compare(const record_t *a, const record_t *b);
store_t *store_create(void);
int store_insert(store_t *store, const record_t *record);
const record_t *store_find(const store_t *store, time_t timestamp);
void store_walk(const store_t *store, store_visit_fn callback, void *user);
size_t store_count(const store_t *store);
/* Not available until Week 8 changes the backing store into a tree. */
int store_height(const store_t *store);
void store_destroy(store_t *store);
#endif
