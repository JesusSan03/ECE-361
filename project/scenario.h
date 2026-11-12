#ifndef SCENARIO_H
#define SCENARIO_H
/* Read a list of changing inputs; values hold between listed ticks. */
#include <stdint.h>
#include <stddef.h>
#include <time.h>
typedef struct { unsigned long tick; float temp; float humidity; uint32_t switches; } change_t;
typedef struct { time_t start; change_t *changes; size_t count; unsigned long last_tick; } scenario_t;
int scenario_load(const char *path, scenario_t *out);
void scenario_free(scenario_t *scenario);
#endif
