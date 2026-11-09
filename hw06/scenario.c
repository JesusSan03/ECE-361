#include "scenario.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int end_of_line(const char *p) {
    while (isspace((unsigned char)*p)) ++p;
    return *p == '\0' || *p == '#';
}
int scenario_load(const char *path, scenario_t *out) {
    if (!path || !out) return 1;
    FILE *f = fopen(path, "r");
    if (!f) return 1;
    scenario_t tmp = {0};
    int have_start = 0, physical = 0, error = 0;
    char line[512];
    while (fgets(line, sizeof line, f)) {
        ++physical;
        if (!strchr(line, '\n') && !feof(f)) { error = physical; break; }
        char *q = line;
        while (isspace((unsigned char)*q)) ++q;
        if (!*q || *q == '#') continue;
        if (!have_start) {
            if (strncmp(q, "start", 5) != 0 || !isspace((unsigned char)q[5])) { error = physical; break; }
            q += 5; while (isspace((unsigned char)*q)) ++q;
            if (!isdigit((unsigned char)*q)) { error = physical; break; }
            errno = 0; char *e;
            unsigned long long v = strtoull(q, &e, 10);
            if (errno || e == q || !end_of_line(e) || v > (unsigned long long)LLONG_MAX || (unsigned long long)(time_t)v != v) { error = physical; break; }
            tmp.start = (time_t)v; have_start = 1;
        } else {
            if (tmp.count == SCENARIO_MAX) { error = physical; break; }
            scenario_line_t x;
            errno = 0; char *e;
            if (!isdigit((unsigned char)*q)) { error = physical; break; }
            unsigned long tick = strtoul(q, &e, 10);
            if (errno || e == q || tick > UINT_MAX || !isspace((unsigned char)*e)) { error = physical; break; }
            q = e; errno = 0; x.temp = strtof(q, &e);
            if (errno || e == q || !isfinite(x.temp) || !isspace((unsigned char)*e)) { error = physical; break; }
            q = e; errno = 0; x.rh = strtof(q, &e);
            if (errno || e == q || !isfinite(x.rh) || !isspace((unsigned char)*e)) { error = physical; break; }
            q = e;
            while (isspace((unsigned char)*q)) ++q;
            if (q[0]!='0' || (q[1]!='x' && q[1]!='X') || !isxdigit((unsigned char)q[2])) { error = physical; break; }
            errno = 0; unsigned long sw = strtoul(q, &e, 16);
            if (errno || e == q || sw > UINT32_MAX || !end_of_line(e)) { error = physical; break; }
            x.tick = (unsigned)tick; x.switches = (uint32_t)sw;
            if (tmp.count && x.tick <= tmp.lines[tmp.count - 1].tick) { error = physical; break; }
            tmp.lines[tmp.count++] = x;
        }
    }
    if (!error && ferror(f)) error = physical ? physical : 1;
    if (!error && (!have_start || !tmp.count)) error = physical ? physical + 1 : 1;
    fclose(f);
    if (!error) *out = tmp;
    return error;
}
const scenario_line_t *scenario_at(const scenario_t *s, unsigned tick) {
    if (!s) return NULL;
    const scenario_line_t *last = NULL;
    for (size_t i = 0; i < s->count; ++i) {
        if (s->lines[i].tick > tick) break;
        last = &s->lines[i];
    }
    return last;
}
