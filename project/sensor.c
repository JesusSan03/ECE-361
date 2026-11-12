#include "sensor.h"
#include "iom361_r4.h"
#include <math.h>
#include <stddef.h>
/* Round to one decimal so a value right on a threshold stays there. */
static float one_decimal(double x) { return (float)(round(x * 10.0) / 10.0); }
int sensor_read(uint32_t *base, time_t when, reading_t *out) {
    if (!base || !out) return -1;
    int rc = 0;
    uint32_t t = iom361_readReg(base, TEMP_REG, &rc);
    if (rc != 0) return -1;
    uint32_t h = iom361_readReg(base, HUMID_REG, &rc);
    if (rc != 0) return -1;
    out->timestamp = when;
    out->temp_c = one_decimal((double)t / 1048576.0 * 200.0 - 50.0);
    out->humidity = one_decimal((double)h / 1048576.0 * 100.0);
    return 0;
}
