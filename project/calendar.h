#ifndef CALENDAR_H
#define CALENDAR_H
/* Format Unix seconds using UTC, not the computer's local time zone. */
#include <time.h>
#include <stddef.h>
int clock_format_utc(time_t moment, char *text, size_t size);
#endif
