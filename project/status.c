#include "status.h"
/* These are the get_field and set_field ideas from HW01. */
uint32_t get_field(uint32_t value, unsigned start, unsigned width) {
    if (width == 0 || width > 32 || start > 31 || width > 32 - start) return 0;
    uint32_t mask = width == 32 ? UINT32_MAX : (UINT32_C(1) << width) - 1u;
    return (value >> start) & mask;
}
uint32_t set_field(uint32_t value, unsigned start, unsigned width, uint32_t field) {
    if (width == 0 || width > 32 || start > 31 || width > 32 - start) return value;
    uint32_t mask = width == 32 ? UINT32_MAX : (UINT32_C(1) << width) - 1u;
    return (value & ~(mask << start)) | ((field & mask) << start);
}
uint32_t status_pack(unsigned state, unsigned leds, unsigned zone) {
    uint32_t result = 0;
    result = set_field(result, 0, 2, state);
    result = set_field(result, 2, 3, leds);
    result = set_field(result, 5, 2, zone);
    return result;
}
void status_unpack(uint32_t word, unsigned *state, unsigned *leds, unsigned *zone) {
    if (state) *state = get_field(word, 0, 2);
    if (leds) *leds = get_field(word, 2, 3);
    if (zone) *zone = get_field(word, 5, 2);
}
