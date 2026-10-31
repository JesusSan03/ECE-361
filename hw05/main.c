#include <stdio.h>
#include "lock.h"

/* Read characters, run valid events, and print each transition and final state. */
int main(void)
{
    state_t state = LOCKED;
    int input;

    while ((input = getchar()) != EOF) {
        int e = lock_event(state, (char)input);
        if (e == -1) continue;

        action_t action;
        state_t next = lock_step(state, (event_t)e, &action);
        printf("%s %c %s -> %s %s\n",
               state_name(state), (char)input, event_name((event_t)e),
               state_name(next), action_name(action));
        state = next;
    }
    printf("final: %s\n", state_name(state));
    return 0;
}
