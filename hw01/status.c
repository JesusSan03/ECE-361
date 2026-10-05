#include <stdint.h>
#include "status.h"
#include "bits.h"

status_t status_unpack(uint16_t word)
{
    status_t status;

    status.heat = get_field(word, 0, 1);
    status.cool = get_field(word, 1, 1);
    status.fan = get_field(word, 2, 1);
    status.fault = get_field(word, 3, 1);
    status.mode = get_field(word, 4, 3);
    status.setpoint = sign_extend(get_field(word, 8, 8), 8);

    return status;
}