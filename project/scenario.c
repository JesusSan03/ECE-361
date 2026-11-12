#include "scenario.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Parse whole lines so comments and incomplete lines cannot confuse us. */
int scenario_load(const char *path, scenario_t *out) {
    if (!path || !out) return -1;
    memset(out, 0, sizeof(*out));
    FILE *fp = fopen(path, "r");
    if (!fp) return -1;
    char line[512];
    int saw_start = 0, result = -1;
    size_t capacity = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (!strchr(line, '\n') && !feof(fp)) goto done; /* overlong line */
        char *hash = strchr(line, '#'); if (hash) *hash = '\0';
        char *s = line; while (isspace((unsigned char)*s)) ++s;
        if (!*s) continue;
        if (!saw_start) {
            long long seconds; char extra;
            if (sscanf(s, "start %lld %c", &seconds, &extra) != 1 || seconds < 0) goto done;
            out->start = (time_t)seconds;
            if ((long long)out->start != seconds) goto done;
            saw_start = 1;
            continue;
        }
        unsigned long tick; float temp, rh; unsigned sw; char extra;
        if (sscanf(s, "%lu %f %f %x %c", &tick, &temp, &rh, &sw, &extra) != 4) goto done;
        if ((out->count && tick <= out->changes[out->count-1].tick) ||
            (!out->count && tick != 0) || rh < 0.f || rh > 100.f ||
            temp < -50.f || temp > 150.f || sw > 3u) goto done;
        if (out->count == capacity) {
            size_t next = capacity ? capacity * 2u : 16u;
            if (next < capacity || next > SIZE_MAX / sizeof(change_t)) goto done;
            change_t *tmp = realloc(out->changes, next * sizeof(change_t));
            if (!tmp) goto done;
            out->changes = tmp; capacity = next;
        }
        out->changes[out->count++] = (change_t){tick, temp, rh, sw};
        out->last_tick = tick;
    }
    if (ferror(fp) || !saw_start || !out->count) goto done;
    result = 0;
done:
    fclose(fp);
    if (result != 0) scenario_free(out);
    return result;
}
void scenario_free(scenario_t *s) {
    if (!s) return;
    free(s->changes); memset(s, 0, sizeof(*s));
}
