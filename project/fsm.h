#ifndef FSM_H
#define FSM_H
/* Our state machine is table-driven. Each event gets one table entry. */
#include <time.h>
typedef enum { IDLE, ZONE_ACTIVE, SOAK, RAIN_HOLD, STATE_COUNT } state_t;
typedef enum { EVENT_NONE, EVENT_START, EVENT_WATER_DONE,
               EVENT_LAST_ZONE_DONE, EVENT_SOAK_DONE,
               EVENT_RAIN_ON, EVENT_RAIN_OFF, EVENT_COUNT } event_t;
typedef enum { ACT_NOTHING, ACT_BEGIN_FIRST, ACT_BEGIN_NEXT,
               ACT_BEGIN_SOAK, ACT_FINISH, ACT_HOLD, ACT_RESUME } action_t;
typedef struct {
    state_t state;
    unsigned zone;          /* 0 means that the controller is idle. */
    unsigned resume_zone;   /* The zone that must restart after rain. */
    time_t phase_start;     /* Clock value when the current phase began. */
} fsm_t;
typedef struct { state_t next; action_t action; } transition_t;
void fsm_init(fsm_t *fsm);
event_t fsm_choose_event(const fsm_t *fsm, time_t now, int start_press, int raining);
state_t fsm_step(fsm_t *fsm, event_t event, time_t now);
const char *fsm_state_name(state_t state);
const transition_t *fsm_transition(state_t state, event_t event);
#endif
