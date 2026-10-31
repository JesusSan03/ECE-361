#include "lock.h"

const transition_t lock_table[N_STATES][N_EVENTS] = {
    [LOCKED] = {
        [E_RIGHT]   = { GOT1,   A_NONE },
        [E_WRONG]   = { LOCKED, A_BEEP },
        [E_CLOSE]   = { LOCKED, A_NONE },
        [E_TIMEOUT] = { LOCKED, A_NONE }
    },
    [GOT1] = {
        [E_RIGHT]   = { GOT2,   A_NONE },
        [E_WRONG]   = { LOCKED, A_BEEP },
        [E_CLOSE]   = { GOT1,   A_NONE },
        [E_TIMEOUT] = { LOCKED, A_NONE }
    },
    [GOT2] = {
        [E_RIGHT]   = { OPEN,   A_UNLOCK },
        [E_WRONG]   = { LOCKED, A_BEEP },
        [E_CLOSE]   = { GOT2,   A_NONE },
        [E_TIMEOUT] = { LOCKED, A_NONE }
    },
    [OPEN] = {
        [E_RIGHT]   = { OPEN,   A_NONE },
        [E_WRONG]   = { OPEN,   A_NONE },
        [E_CLOSE]   = { LOCKED, A_LOCK },
        [E_TIMEOUT] = { LOCKED, A_LOCK }
    }
};

state_t lock_step(state_t s, event_t e, action_t *action)
{
    transition_t entry = lock_table[s][e];
    if (action != 0) {
        *action = entry.action;
    }
    return entry.next;
}

int lock_event(state_t s, char c)
{
    if (c == 'c') return E_CLOSE;
    if (c == 't') return E_TIMEOUT;
    if (c >= '0' && c <= '9') {
        static const char expected[N_STATES] = { '3', '1', '4', '\0' };
        return (s == OPEN || c == expected[s]) ? E_RIGHT : E_WRONG;
    }
    return -1;
}

/* Look up the event name safely. */
const char *state_name(state_t s)
{
    static const char *const names[N_STATES] = { "LOCKED", "GOT1", "GOT2", "OPEN" };
    return (s >= 0 && s < N_STATES) ? names[s] : "?";
}

const char *event_name(event_t e)
{
    static const char *const names[N_EVENTS] = { "RIGHT", "WRONG", "CLOSE", "TIMEOUT" };
    return (e >= 0 && e < N_EVENTS) ? names[e] : "?";
}

const char *action_name(action_t a)
{
    static const char *const names[N_ACTIONS] = { "UNDEFINED", "NONE", "UNLOCK", "LOCK", "BEEP" };
    return (a >= 0 && a < N_ACTIONS) ? names[a] : "?";
}
