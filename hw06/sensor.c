#include "sensor.h"
#include "iom361_r4.h"
#include <math.h>
int sensor_read(uint32_t *base, time_t now, reading_t *r) {
    if (!r) return 1;
    int rc = 0;
    uint32_t st = iom361_readReg(base, TEMP_REG, &rc);
    if (rc) return rc;
    uint32_t srh = iom361_readReg(base, HUMID_REG, &rc);
    if (rc) return rc;
    reading_t tmp;
    tmp.t = now;
    tmp.temp_c = roundf((((st & 0xFFFFFu) / 1048576.0f) * 200.0f - 50.0f) * 10.0f) / 10.0f;
    tmp.rh_pct = roundf((((srh & 0xFFFFFu) / 1048576.0f) * 100.0f) * 10.0f) / 10.0f;
    *r = tmp;
    return 0;
}
