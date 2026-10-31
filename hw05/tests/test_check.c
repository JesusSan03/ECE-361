#include <stdio.h>
#include <string.h>
#include "check.h"

static int passed = 0, failed = 0;

/* Record a single assertion. */
static void expect(int condition, const char *label)
{
    printf("%s: %s\n", condition ? "PASS" : "FAIL", label);
    if (condition) ++passed; else ++failed;
}

/* Restore an editable copy of the valid lock table. */
static void copy_table(transition_t dst[N_STATES][N_EVENTS])
{
    memcpy(dst, lock_table, sizeof(lock_table));
}

/* Exercise the checker against valid and deliberately broken state tables. */
int main(void)
{
    transition_t copy[N_STATES][N_EVENTS];
    copy_table(copy);
    expect(fsm_check((const transition_t (*)[N_EVENTS])copy, LOCKED, stdout) == 0, "valid table has zero problems");

    copy_table(copy);
    copy[GOT1][E_CLOSE].action = A_UNDEFINED;
    expect(fsm_check((const transition_t (*)[N_EVENTS])copy, LOCKED, stdout) == 1, "detects an undefined action");

    copy_table(copy);
    copy[GOT2][E_RIGHT].next = N_STATES;
    expect(fsm_check((const transition_t (*)[N_EVENTS])copy, LOCKED, stdout) >= 1, "detects out-of-range next state");

    copy_table(copy);
    copy[GOT1][E_RIGHT].next = GOT1;
    expect(fsm_check((const transition_t (*)[N_EVENTS])copy, LOCKED, stdout) >= 1, "detects unreachable GOT2 and OPEN");

    copy_table(copy);
    for (int e = 0; e < N_EVENTS; ++e) {
        copy[OPEN][e].next = OPEN;
    }
    expect(fsm_check((const transition_t (*)[N_EVENTS])copy, LOCKED, stdout) >= 1, "detects an OPEN state with no exit");

    printf("SUMMARY: %d passed, %d failed\n", passed, failed);
    return failed ? 1 : 0;
}
