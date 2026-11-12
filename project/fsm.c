#include "fsm.h"
#include <stddef.h>
/* Every state/event pair has an explicit next state and action.
 * Events that do not make sense in a state leave it unchanged. */
static const transition_t transitions[STATE_COUNT][EVENT_COUNT] = {
    [IDLE] = {
        [EVENT_NONE] = {IDLE, ACT_NOTHING}, [EVENT_START] = {ZONE_ACTIVE, ACT_BEGIN_FIRST},
        [EVENT_WATER_DONE] = {IDLE, ACT_NOTHING}, [EVENT_LAST_ZONE_DONE] = {IDLE, ACT_NOTHING},
        [EVENT_SOAK_DONE] = {IDLE, ACT_NOTHING}, [EVENT_RAIN_ON] = {IDLE, ACT_NOTHING},
        [EVENT_RAIN_OFF] = {IDLE, ACT_NOTHING}
    },
    [ZONE_ACTIVE] = {
        [EVENT_NONE] = {ZONE_ACTIVE, ACT_NOTHING}, [EVENT_START] = {ZONE_ACTIVE, ACT_NOTHING},
        [EVENT_WATER_DONE] = {SOAK, ACT_BEGIN_SOAK},
        [EVENT_LAST_ZONE_DONE] = {IDLE, ACT_FINISH},
        [EVENT_SOAK_DONE] = {ZONE_ACTIVE, ACT_NOTHING},
        [EVENT_RAIN_ON] = {RAIN_HOLD, ACT_HOLD},
        [EVENT_RAIN_OFF] = {ZONE_ACTIVE, ACT_NOTHING}
    },
    [SOAK] = {
        [EVENT_NONE] = {SOAK, ACT_NOTHING}, [EVENT_START] = {SOAK, ACT_NOTHING},
        [EVENT_WATER_DONE] = {SOAK, ACT_NOTHING}, [EVENT_LAST_ZONE_DONE] = {SOAK, ACT_NOTHING},
        [EVENT_SOAK_DONE] = {ZONE_ACTIVE, ACT_BEGIN_NEXT},
        [EVENT_RAIN_ON] = {RAIN_HOLD, ACT_HOLD},
        [EVENT_RAIN_OFF] = {SOAK, ACT_NOTHING}
    },
    [RAIN_HOLD] = {
        [EVENT_NONE] = {RAIN_HOLD, ACT_NOTHING}, [EVENT_START] = {RAIN_HOLD, ACT_NOTHING},
        [EVENT_WATER_DONE] = {RAIN_HOLD, ACT_NOTHING},
        [EVENT_LAST_ZONE_DONE] = {RAIN_HOLD, ACT_NOTHING},
        [EVENT_SOAK_DONE] = {RAIN_HOLD, ACT_NOTHING},
        [EVENT_RAIN_ON] = {RAIN_HOLD, ACT_NOTHING},
        [EVENT_RAIN_OFF] = {ZONE_ACTIVE, ACT_RESUME}
    }
};
void fsm_init(fsm_t *fsm) {
    if (!fsm) return;
    fsm->state = IDLE; fsm->zone = 0;
    fsm->resume_zone = 0; fsm->phase_start = 0;
}
const transition_t *fsm_transition(state_t state, event_t event) {
    if (state < 0 || state >= STATE_COUNT || event < 0 || event >= EVENT_COUNT) return NULL;
    return &transitions[state][event];
}
event_t fsm_choose_event(const fsm_t *fsm, time_t now, int start_press, int raining) {
    if (!fsm) return EVENT_NONE;
    /* Rain has priority over every timer and button press. */
    if (fsm->state == RAIN_HOLD) return raining ? EVENT_NONE : EVENT_RAIN_OFF;
    if (raining && (fsm->state == ZONE_ACTIVE || fsm->state == SOAK)) return EVENT_RAIN_ON;
    if (fsm->state == IDLE) return (!raining && start_press) ? EVENT_START : EVENT_NONE;
    if (fsm->state == ZONE_ACTIVE && difftime(now, fsm->phase_start) >= 120.0)
        return fsm->zone == 3 ? EVENT_LAST_ZONE_DONE : EVENT_WATER_DONE;
    if (fsm->state == SOAK && difftime(now, fsm->phase_start) >= 60.0)
        return EVENT_SOAK_DONE;
    return EVENT_NONE;
}
state_t fsm_step(fsm_t *fsm, event_t event, time_t now) {
    if (!fsm) return IDLE;
    const transition_t *t = fsm_transition(fsm->state, event);
    if (!t) return fsm->state;
    /* Save the interrupted zone BEFORE we change the state. */
    if (t->action == ACT_HOLD)
        fsm->resume_zone = fsm->state == SOAK ? fsm->zone + 1u : fsm->zone;
    fsm->state = t->next;
    switch (t->action) {
        case ACT_BEGIN_FIRST: fsm->zone = 1; fsm->phase_start = now; break;
        case ACT_BEGIN_NEXT: ++fsm->zone; fsm->phase_start = now; break;
        case ACT_BEGIN_SOAK: fsm->phase_start = now; break;
        case ACT_RESUME: fsm->zone = fsm->resume_zone; fsm->phase_start = now; break;
        case ACT_FINISH: fsm->zone = 0; fsm->resume_zone = 0; break;
        case ACT_HOLD: case ACT_NOTHING: break;
    }
    return fsm->state;
}
const char *fsm_state_name(state_t state) {
    switch (state) {
        case IDLE: return "IDLE";
        case ZONE_ACTIVE: return "ZONE_ACTIVE";
        case SOAK: return "SOAK";
        case RAIN_HOLD: return "RAIN_HOLD";
        default: return "UNKNOWN";
    }
}
