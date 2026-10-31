#include <stdio.h>
#include "lock.h"

static int passed = 0, failed = 0;

/* Check a boolean expectation and log a separate pass or fail. */
static void expect(int ok, const char *name)
{
    printf("%s: %s\n", ok ? "PASS" : "FAIL", name);
    if (ok) ++passed; else ++failed;
}

/* Verify the entire transition table, including next state and action. */
static void test_table(void)
{
    static const transition_t expected[N_STATES][N_EVENTS] = {
        { {GOT1,A_NONE}, {LOCKED,A_BEEP}, {LOCKED,A_NONE}, {LOCKED,A_NONE} },
        { {GOT2,A_NONE}, {LOCKED,A_BEEP}, {GOT1,A_NONE}, {LOCKED,A_NONE} },
        { {OPEN,A_UNLOCK}, {LOCKED,A_BEEP}, {GOT2,A_NONE}, {LOCKED,A_NONE} },
        { {OPEN,A_NONE}, {OPEN,A_NONE}, {LOCKED,A_LOCK}, {LOCKED,A_LOCK} }
    };
    for (int s = 0; s < N_STATES; ++s) {
        for (int e = 0; e < N_EVENTS; ++e) {
            action_t action = A_UNDEFINED;
            state_t next = lock_step((state_t)s, (event_t)e, &action);
            char label[100];
            snprintf(label, sizeof(label), "table %s / %s", state_name((state_t)s), event_name((event_t)e));
            expect(next == expected[s][e].next && action == expected[s][e].action, label);
        }
    }
}

/* Verify character classification in each state and special inputs. */
static void test_events(void)
{
    static const char correct[N_STATES] = {'3','1','4','3'};
    static const char wrong[N_STATES] = {'1','3','1','9'};
    for (int s = 0; s < N_STATES; ++s) {
        char label[100];
        int right = lock_event((state_t)s, correct[s]);
        int bad = lock_event((state_t)s, wrong[s]);
        snprintf(label, sizeof(label), "right digit in %s", state_name((state_t)s));
        expect(right == E_RIGHT, label);
        snprintf(label, sizeof(label), "wrong digit in %s", state_name((state_t)s));
        expect(bad == (s == OPEN ? E_RIGHT : E_WRONG), label);
        snprintf(label, sizeof(label), "close in %s", state_name((state_t)s));
        expect(lock_event((state_t)s, 'c') == E_CLOSE, label);
        snprintf(label, sizeof(label), "timeout in %s", state_name((state_t)s));
        expect(lock_event((state_t)s, 't') == E_TIMEOUT, label);
        snprintf(label, sizeof(label), "other input in %s", state_name((state_t)s));
        expect(lock_event((state_t)s, 'x') == -1, label);
    }
}

/* Run a full input sequence and check the final state. */
static void test_sequence(const char *input, state_t goal, const char *label)
{
    state_t s = LOCKED;
    for (const char *p = input; *p; ++p) {
        int e = lock_event(s, *p);
        if (e != -1) s = lock_step(s, (event_t)e, NULL);
    }
    expect(s == goal, label);
}

/* Run all required tests and return nonzero when any fail. */
int main(void)
{
    test_table();
    test_events();
    test_sequence("314", OPEN, "314 opens");
    test_sequence("3314", LOCKED, "3314 does not open");
    test_sequence("31t4", LOCKED, "31t4 does not open");
    test_sequence("314c", LOCKED, "314c locks again");
    test_sequence("31c4", OPEN, "closing mid-code does nothing");
    test_sequence("3149", OPEN, "digits do nothing while open");
    test_sequence("314t", LOCKED, "timeout locks while open");
    expect(lock_event(LOCKED, '\n') == -1, "newlines ignored");
    expect(lock_event(GOT2, 'q') == -1, "non-special letters ignored");
    expect(lock_step(LOCKED, E_CLOSE, NULL) == LOCKED, "NULL action pointer accepted");
    expect(state_name((state_t)-1)[0] == '?', "invalid state name");
    expect(event_name((event_t)N_EVENTS)[0] == '?', "invalid event name");
    expect(action_name((action_t)N_ACTIONS)[0] == '?', "invalid action name");
    printf("SUMMARY: %d passed, %d failed\n", passed, failed);
    return failed ? 1 : 0;
}
