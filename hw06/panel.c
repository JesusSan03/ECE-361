#include "panel.h"
#include "iom361_r4.h"
uint32_t panel_pressed(uint32_t now, uint32_t before) { return now & ~before; }
uint32_t panel_released(uint32_t now, uint32_t before) { return before & ~now; }
int panel_led(uint32_t *base, int n, int on) {
    if (n < 0 || n > 31) return -1;
    int rc = 0;
    uint32_t old = iom361_readReg(base, LEDS_REG, &rc);
    if (rc) return rc;
    uint32_t mask = (uint32_t)1u << (unsigned)n;
    uint32_t next = on ? (old | mask) : (old & ~mask);
    (void)iom361_writeReg(base, LEDS_REG, next, &rc);
    return rc;
}
int panel_toggle(uint32_t *base, int n) {
    if (n < 0 || n > 31) return -1;
    int rc = 0;
    uint32_t old = iom361_readReg(base, LEDS_REG, &rc);
    if (rc) return rc;
    (void)iom361_writeReg(base, LEDS_REG, old ^ ((uint32_t)1u << (unsigned)n), &rc);
    return rc;
}
