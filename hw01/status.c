#include <stdint.h>
#include "status.h"
#include "bits.h"

status_t status_unpack(uint16_t word)
{
    status_t status;

    status.heat = get_field(word, HEAT_POS, HEAT_WIDTH);
    /* Gets the heat bit at bit 0 */

    status.cool = get_field(word, COOL_POS, COOL_WIDTH);
    /* Gets the cool bit at bit 1 */

    status.fan = get_field(word, FAN_POS, FAN_WIDTH);
    /* Gets the fan bit at bit 2 */

    status.fault = get_field(word, FAULT_POS, FAULT_WIDTH);
    /* Gets the fault bit at bit 3 */

    status.mode = get_field(word, MODE_POS, MODE_WIDTH);
    /* Gets the 3 mode bits from bits 4 through 6 */

    status.reserved = get_field(word, RESERVED_POS, RESERVED_WIDTH);
    /* Gets the reserved bit at bit 7 */

    status.setpoint =
        sign_extend(get_field(word, SETPOINT_POS, SETPOINT_WIDTH),
                    SETPOINT_WIDTH);
    /* Gets bits 8 through 15 for the setpoint, then sign extends it
       because the setpoint is an 8-bit signed value */

    return status;
}