#ifndef SCENARIO_H
#define SCENARIO_H
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#define SCENARIO_MAX 1000
typedef struct { unsigned tick; float temp; float rh; uint32_t switches; } scenario_line_t;
typedef struct { time_t start; size_t count; scenario_line_t lines[SCENARIO_MAX]; } scenario_t;
/* Returns 0 on success, first invalid physical line number otherwise. */
int scenario_load(const char *path, scenario_t *out);
/* The most recent line at or before tick; NULL before the first line. */
const scenario_line_t *scenario_at(const scenario_t *s, unsigned tick);
#endif
