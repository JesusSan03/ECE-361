#ifndef SENSOR_H
#define SENSOR_H
/* One sensor reading and its UTC timestamp. */
#include <stdint.h>
#include <time.h>
typedef struct { time_t timestamp; float temp_c; float humidity; } reading_t;
/* Return 0 on success and -1 for a bad argument or hardware error. */
int sensor_read(uint32_t *base, time_t when, reading_t *out);
#endif
