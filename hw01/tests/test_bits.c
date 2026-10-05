#include <stdio.h>
#include <stdint.h>
#include "../bits.h"
#include "../status.h"

int main(void)
{
    printf("=== get_field tests ===\n");

    printf("width 3: %u\n", get_field(0b11010110, 2, 3));
    printf("width 1: %u\n", get_field(0x80000000u, 31, 1));
    printf("width 32: %u\n", get_field(0x12345678u, 0, 32));
    printf("position 31: %u\n", get_field(0x80000000u, 31, 1));


    printf("\n=== print_binary test ===\n");

    printf("0x2C as 8 bits: ");
    print_binary(0x2C, 8);


    printf("\n=== set_field tests ===\n");

    uint32_t set_result = set_field(0b11010110, 2, 3, 3);
    printf("normal set_field: ");
    print_binary(set_result, 8);

    uint32_t wide_value = set_field(0, 2, 3, 0xFF);
    printf("value too wide: ");
    print_binary(wide_value, 8);

    uint32_t full_width = set_field(0xAAAAAAAAu, 0, 32, 0x12345678u);
    printf("width 32: ");
    print_binary(full_width, 32);


    printf("\n=== sign_extend tests ===\n");

    printf("0xF8 as 8-bit signed: %d\n", sign_extend(0xF8, 8));

    printf("most negative 8-bit value: %d\n",
           sign_extend(0x80, 8));

    printf("positive 8-bit value: %d\n",
           sign_extend(0x7F, 8));


    printf("\n=== status_unpack tests ===\n");

    status_t s1 = status_unpack(0x1631);

    printf("0x1631:\n");
    printf("setpoint: %d\n", s1.setpoint);
    printf("mode: %u\n", s1.mode);
    printf("fault: %u\n", s1.fault);
    printf("fan: %u\n", s1.fan);
    printf("cool: %u\n", s1.cool);
    printf("heat: %u\n", s1.heat);


    status_t s2 = status_unpack(0x0012);

    printf("\n0x0012:\n");
    printf("setpoint: %d\n", s2.setpoint);
    printf("mode: %u\n", s2.mode);
    printf("fault: %u\n", s2.fault);
    printf("fan: %u\n", s2.fan);
    printf("cool: %u\n", s2.cool);
    printf("heat: %u\n", s2.heat);


    status_t s3 = status_unpack(0xFF0D);

    printf("\n0xFF0D:\n");
    printf("setpoint: %d\n", s3.setpoint);
    printf("mode: %u\n", s3.mode);
    printf("fault: %u\n", s3.fault);
    printf("fan: %u\n", s3.fan);
    printf("cool: %u\n", s3.cool);
    printf("heat: %u\n", s3.heat);


    return 0;
}