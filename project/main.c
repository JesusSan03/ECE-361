
#include <stdio.h>
#include "iom361_r4.h"
#include "sensor.h"
#include "fsm.h"
#include "store.h"
#include "aux.h"
#include "report.h"

int main(void) {
    int rc = 0;

    // Set up the irrigation system with 2 switches and 3 LEDs.
    uint32_t *base = iom361_initialize(2, 3, &rc);

    if (base == NULL || rc != 0) {
        fprintf(stderr, "iom361_initialize failed: %d\n", rc);
        return 1;
    }

    // Print the name of the device.
    printf("Irrigation controller\n");

    return 0;
}
