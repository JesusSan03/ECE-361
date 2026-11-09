#include "iom361_r4.h"
#include "sensor.h"
#include "panel.h"
#include "scenario.h"
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, char **argv) {
    if (argc != 2) { fprintf(stderr, "Usage: %s scenario.txt\n", argv[0]); return 2; }
    scenario_t *sc = malloc(sizeof *sc);
    if (!sc) { fputs("Out of memory\n", stderr); return 1; }
    int bad = scenario_load(argv[1], sc);
    if (bad) { fprintf(stderr, "Invalid scenario at line %d\n", bad); free(sc); return 1; }
    int rc = 0;
    uint32_t *base = iom361_initialize(4, 4, &rc);
    if (!base || rc) { free(sc); return 1; }
    iom361_set_display(false);
    uint32_t before = 0;
    const unsigned last = sc->lines[sc->count-1].tick;
    for (unsigned tick = 0; ; ++tick) {
        const scenario_line_t *line = scenario_at(sc, tick);
        if (line) { _iom361_set_sensor1(line->temp, line->rh); _iom361_set_switches(line->switches); }
        if ((uintmax_t)sc->start + tick > (uintmax_t)LLONG_MAX) { fputs("time overflow\n", stderr); free(sc); return 1; }
        time_t now = sc->start + (time_t)tick;
        reading_t r;
        if (sensor_read(base, now, &r)) { fputs("sensor read failed\n", stderr); free(sc); return 1; }
        uint32_t switches = iom361_readReg(base, SWITCHES_REG, &rc);
        if (rc) { free(sc); return 1; }
        uint32_t pressed = panel_pressed(switches, before);
        for (int k = 0; k < 4; ++k) {
            if (pressed & (1u << k)) {
                if (panel_toggle(base, k)) { free(sc); return 1; }
            }
        }
        uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);
        if (rc) { free(sc); return 1; }
        struct tm *utc = gmtime(&now);
        char stamp[32];
        if (!utc || !strftime(stamp, sizeof stamp, "%Y-%m-%d %H:%M:%S", utc)) { free(sc); return 1; }
        printf("%s temp=%.1f rh=%.1f switches=0x%" PRIx32 " pressed=0x%" PRIx32 " leds=0x%" PRIx32 "\n", stamp, r.temp_c, r.rh_pct, switches, pressed, leds);
        before = switches;
        if (tick == last) break;
    }
    free(sc);
    return 0;
}
