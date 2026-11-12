#include "aux.h"
#include "calendar.h"
#include "fsm.h"
#include "iom361_r4.h"
#include "report.h"
#include "scenario.h"
#include "sensor.h"
#include "status.h"
#include "store.h"
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
/* Only main and tests feed fake-world values into the hardware model. */
static unsigned valve_bits(const fsm_t *fsm) {
    return fsm->state == ZONE_ACTIVE && fsm->zone >= 1 && fsm->zone <= 3
        ? 1u << (fsm->zone - 1u) : 0u;
}
static uint32_t rgb_for_state(state_t state) {
    /* IDLE=blue, watering=green, SOAK=yellow, rain hold=red. */
    switch (state) {
        case IDLE: return build_rgb_reg(1, 0, 0, 255);
        case ZONE_ACTIVE: return build_rgb_reg(1, 0, 255, 0);
        case SOAK: return build_rgb_reg(1, 255, 255, 0);
        case RAIN_HOLD: return build_rgb_reg(1, 255, 0, 0);
        default: return build_rgb_reg(0, 0, 0, 0);
    }
}
static int replay(const scenario_t *sc, int report_only) {
    int rc = 0, result = 1;
    uint32_t *base = iom361_initialize(2, 3, &rc);
    if (!base || rc) { fputs("hardware init failed\n", stderr); return 1; }
    iom361_set_display(false); /* The program trace must be the only output. */
    store_t *store = store_create();
    aux_t *history = aux_create();
    if (!store || !history) { fputs("out of memory\n", stderr); goto done; }
    fsm_t fsm; fsm_init(&fsm);
    uint32_t previous_switches = 0;
    size_t change_index = 0;
    for (unsigned long tick = 0; ; ++tick) {
        /* The last settings continue until the next scenario change. */
        if (change_index < sc->count && sc->changes[change_index].tick == tick) {
            const change_t *change = &sc->changes[change_index++];
            _iom361_set_sensor1(change->temp, change->humidity);
            _iom361_set_switches(change->switches);
        }
        /* Check that the Unix time calculation did not wrap. */
        time_t stamp = sc->start + (time_t)tick;
        if ((unsigned long)(stamp - sc->start) != tick) { fputs("clock overflow\n", stderr); goto done; }
        reading_t reading;
        if (sensor_read(base, stamp, &reading)) { fputs("sensor error\n", stderr); goto done; }
        uint32_t switches = iom361_readReg(base, SWITCHES_REG, &rc);
        if (rc) { fputs("switch read failed\n", stderr); goto done; }
        int pressed = (switches & 1u) && !(previous_switches & 1u);
        int raining = (switches & 2u) || reading.humidity >= 90.0f;
        previous_switches = switches;
        event_t event = fsm_choose_event(&fsm, stamp, pressed, raining);
        fsm_step(&fsm, event, stamp);
        unsigned valves = valve_bits(&fsm);
        iom361_writeReg(base, LEDS_REG, valves, &rc);
        if (rc) { fputs("LED write failed\n", stderr); goto done; }
        iom361_writeReg(base, RGB_LED_REG, rgb_for_state(fsm.state), &rc);
        if (rc) { fputs("RGB write failed\n", stderr); goto done; }
        uint32_t status = status_pack((unsigned)fsm.state, valves, fsm.zone);
        record_t record = {reading, fsm.state, status};
        if (store_insert(store, &record) || aux_add(history, &reading)) {
            fputs("record storage failed\n", stderr); goto done;
        }
        if (!report_only) {
            char when[32];
            if (clock_format_utc(stamp, when, sizeof(when))) goto done;
            printf("%s state=%s temp=%.1f rh=%.1f leds=0x%X status=0x%08" PRIX32 "\n",
                when, fsm_state_name(fsm.state), reading.temp_c, reading.humidity,
                valves, status);
        }
        if (tick == sc->last_tick) break;
        if (tick == ULONG_MAX) break;
    }
    if (report_only && report_print(stdout, store, 8.0f)) {
        fputs("Report is a Week 9 feature and is not implemented yet.\n", stderr);
        goto done;
    }
    result = 0;
done:
    aux_destroy(history); store_destroy(store);
    return result;
}
int main(int argc, char **argv) {
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Usage: %s scenario.txt [--report]\n", argv[0]); return 1;
    }
    int report_only = argc == 3;
    if (report_only && strcmp(argv[2], "--report")) {
        fputs("Unknown option (expected --report)\n", stderr); return 1;
    }
    scenario_t sc;
    if (scenario_load(argv[1], &sc)) {
        fprintf(stderr, "Cannot read valid scenario: %s\n", argv[1]); return 1;
    }
    int outcome = replay(&sc, report_only);
    scenario_free(&sc);
    return outcome;
}
