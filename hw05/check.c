#include "check.h"

int fsm_check(const transition_t table[N_STATES][N_EVENTS], state_t start, FILE *out)
{
    int problems = 0;
    int visited[N_STATES] = { 0 };
    state_t queue[N_STATES];
    int front = 0, back = 0;

    if (out == NULL) out = stdout;
    if (start < 0 || start >= N_STATES) {
        fprintf(out, "invalid starting state: %d\n", (int)start);
        return 1;
    }

    for (int s = 0; s < N_STATES; ++s) {
        int has_exit = 0;
        for (int e = 0; e < N_EVENTS; ++e) {
            transition_t entry = table[s][e];
            if (entry.action == A_UNDEFINED) {
                fprintf(out, "undefined entry: %s / %s\n",
                        state_name((state_t)s), event_name((event_t)e));
                ++problems;
            }
            if (entry.next < 0 || entry.next >= N_STATES) {
                fprintf(out, "out-of-range next state: %s / %s -> %d\n",
                        state_name((state_t)s), event_name((event_t)e), (int)entry.next);
                ++problems;
            } else if (entry.next != (state_t)s) {
                has_exit = 1;
            }
        }
        if (!has_exit) {
            fprintf(out, "no way out: %s\n", state_name((state_t)s));
            ++problems;
        }
    }

    visited[start] = 1;
    queue[back++] = start;
    while (front < back) {
        state_t s = queue[front++];
        for (int e = 0; e < N_EVENTS; ++e) {
            state_t next = table[s][e].next;
            if (next >= 0 && next < N_STATES && !visited[next]) {
                visited[next] = 1;
                queue[back++] = next;
            }
        }
    }
    for (int s = 0; s < N_STATES; ++s) {
        if (!visited[s]) {
            fprintf(out, "unreachable state: %s\n", state_name((state_t)s));
            ++problems;
        }
    }
    return problems;
}
