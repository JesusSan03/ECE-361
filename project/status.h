#ifndef STATUS_H
#define STATUS_H
/* Bits 0-1 state, 2-4 valves, 5-6 zone; all higher bits stay zero. */
#include <stdint.h>
uint32_t get_field(uint32_t value, unsigned start, unsigned width);
uint32_t set_field(uint32_t value, unsigned start, unsigned width, uint32_t field);
uint32_t status_pack(unsigned state, unsigned leds, unsigned zone);
void status_unpack(uint32_t word, unsigned *state, unsigned *leds, unsigned *zone);
#endif
