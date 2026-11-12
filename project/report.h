#ifndef REPORT_H
#define REPORT_H
/* Week 9 will add the humidity rate-of-change report here. */
#include "store.h"
#include <stdio.h>
int report_print(FILE *out, const store_t *store, float threshold);
#endif
