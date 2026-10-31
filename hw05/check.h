#ifndef CHECK_H
#define CHECK_H

#include <stdio.h>
#include "lock.h"

/* Report wrong entries, unreachable states, and states without an exit. */
int fsm_check(const transition_t table[N_STATES][N_EVENTS], state_t start, FILE *out);

#endif
