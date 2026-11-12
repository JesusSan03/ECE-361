#include "aux.h"
#include "calendar.h"
#include "fsm.h"
#include "iom361_r4.h"
#include "scenario.h"
#include "sensor.h"
#include "status.h"
#include "store.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static int visits;
static time_t last_key;
static void check_order(const record_t *r, void *ctx) {
    (void)ctx;
    if (visits) assert(r->reading.timestamp > last_key);
    last_key = r->reading.timestamp; ++visits;
}
static void test_sensor(void) {
    int rc;
    uint32_t *base = iom361_initialize(2, 3, &rc);
    assert(base && !rc);
    reading_t r;
    _iom361_set_sensor1(0, 90);
    assert(sensor_read(base, 12, &r) == 0);
    assert(r.timestamp == 12 && r.temp_c == 0.f && r.humidity == 90.f);
    _iom361_set_sensor1(100, 50);
    assert(sensor_read(base, 13, &r) == 0 && r.temp_c == 100.f);
    assert(sensor_read(NULL, 0, &r) == -1);
}
static void test_fsm(void) {
    fsm_t f; fsm_init(&f);
    assert(fsm_choose_event(&f, 0, 1, 0) == EVENT_START);
    fsm_step(&f, EVENT_START, 0);
    assert(f.state == ZONE_ACTIVE && f.zone == 1);
    assert(fsm_choose_event(&f, 119, 0, 0) == EVENT_NONE);
    assert(fsm_choose_event(&f, 120, 0, 0) == EVENT_WATER_DONE);
    fsm_step(&f, EVENT_WATER_DONE, 120);
    assert(f.state == SOAK);
    fsm_step(&f, EVENT_RAIN_ON, 121);
    assert(f.state == RAIN_HOLD && f.resume_zone == 2);
    fsm_step(&f, EVENT_RAIN_OFF, 130);
    assert(f.state == ZONE_ACTIVE && f.zone == 2 && f.phase_start == 130);
    fsm_step(&f, EVENT_RAIN_ON, 150);
    fsm_step(&f, EVENT_RAIN_OFF, 180);
    assert(f.zone == 2 && f.phase_start == 180);
    fsm_step(&f, EVENT_WATER_DONE, 300);
    fsm_step(&f, EVENT_SOAK_DONE, 360);
    assert(f.zone == 3);
    fsm_step(&f, EVENT_LAST_ZONE_DONE, 480);
    assert(f.state == IDLE && f.zone == 0);
}
static void test_status(void) {
    for (unsigned s = 0; s < 4; ++s)
        for (unsigned z = 0; z < 4; ++z)
            for (unsigned led = 0; led < 8; ++led) {
                uint32_t word = status_pack(s, led, z);
                unsigned ss, ll, zz;
                status_unpack(word, &ss, &ll, &zz);
                assert(s == ss && z == zz && led == ll && (word & ~127u) == 0);
            }
}
static void test_store(void) {
    record_t a = {{10, 2, 3}, IDLE, 0};
    record_t b = {{20, 3, 4}, SOAK, 0};
    assert(record_compare(&a, &b) < 0 && record_compare(&b, &a) > 0);
    assert(record_compare(&a, &a) == 0);
    store_t *store = store_create(); assert(store);
    assert(store_insert(store, &b) == 0 && store_insert(store, &a) == 0);
    assert(store_insert(store, &b) == -1);
    assert(store_find(store, 10) && !store_find(store, 9));
    for (int i = 30; i < 130; ++i) {
        record_t c = {{i, 0, 0}, IDLE, 0};
        assert(store_insert(store, &c) == 0);
    }
    assert(store_count(store) == 102 && store_height(store) == -1);
    visits = 0; store_walk(store, check_order, NULL); assert(visits == 102);
    store_destroy(store); store_destroy(NULL);
}
static void test_aux(void) {
    aux_t *b = aux_create(); assert(b);
    reading_t r;
    assert(aux_get_newest(b, 0, &r) == -1);
    for (int i = 0; i < 20; ++i) {
        reading_t item = {i, (float)i, 60};
        assert(aux_add(b, &item) == 0);
    }
    assert(aux_count(b) == 16 && aux_is_full(b));
    for (int i = 0; i < 16; ++i) {
        assert(aux_get_newest(b, (size_t)i, &r) == 0);
        assert(r.timestamp == 19 - i);
    }
    assert(aux_get_newest(b, 16, &r) == -1);
    aux_destroy(b);
}
static void test_clock(void) {
    char out[40];
    assert(clock_format_utc(1791273600, out, sizeof(out)) == 0);
    assert(strcmp(out, "2026-10-06 08:00:00") == 0);
    assert(clock_format_utc(1791273659, out, sizeof(out)) == 0);
    assert(strcmp(out, "2026-10-06 08:00:59") == 0);
    /* 2027-02-28 23:59:59 UTC */
    assert(clock_format_utc(1803859199, out, sizeof(out)) == 0);
    assert(strcmp(out, "2027-02-28 23:59:59") == 0);
    assert(clock_format_utc(1803859200, out, sizeof(out)) == 0);
    assert(strcmp(out, "2027-03-01 00:00:00") == 0);
}
int main(void) {
    test_sensor(); puts("PASS unit: sensor");
    test_fsm(); puts("PASS unit: FSM");
    test_status(); puts("PASS unit: status");
    test_store(); puts("PASS unit: sorted store");
    test_aux(); puts("PASS unit: circular buffer");
    test_clock(); puts("PASS unit: UTC clock");
    puts("Unit test groups passed: 6, failed: 0");
    return 0;
}
