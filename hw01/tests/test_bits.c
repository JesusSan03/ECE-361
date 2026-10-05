#include <stdio.h>
#include <stdint.h>
#include "../bits.h"
#include "../status.h"

static int tests_run = 0;
static int tests_failed = 0;

static void test_uint32(const char *name, uint32_t actual, uint32_t expected)
{
    tests_run++;

    if (actual == expected)
    {
        printf("PASS: %s\n", name);
    }
    else
    {
        printf("FAIL: %s (expected %u, got %u)\n",
               name, expected, actual);
        tests_failed++;
    }
}

static void test_int32(const char *name, int32_t actual, int32_t expected)
{
    tests_run++;

    if (actual == expected)
    {
        printf("PASS: %s\n", name);
    }
    else
    {
        printf("FAIL: %s (expected %d, got %d)\n",
               name, expected, actual);
        tests_failed++;
    }
}

int main(void)
{
    /* get_field tests */
    test_uint32(
        "get_field normal",
        get_field(0xD6u, 2, 3),
        5u
    );

    test_uint32(
        "get_field width 1",
        get_field(0x80000000u, 31, 1),
        1u
    );

    test_uint32(
        "get_field width 32",
        get_field(0x12345678u, 0, 32),
        0x12345678u
    );

    test_uint32(
        "get_field position 31",
        get_field(0x80000000u, 31, 1),
        1u
    );

    test_uint32(
        "get_field invalid width",
        get_field(0x12345678u, 0, 33),
        0u
    );


    /* set_field tests */
    test_uint32(
        "set_field normal",
        set_field(0xD6u, 2, 3, 3u),
        0xCEu
    );

    test_uint32(
        "set_field value too wide",
        set_field(0u, 2, 3, 0xFFu),
        0x1Cu
    );

    test_uint32(
        "set_field width 32",
        set_field(0xAAAAAAAAu, 0, 32, 0x12345678u),
        0x12345678u
    );

    test_uint32(
        "set_field invalid field returns original word",
        set_field(0x12345678u, 31, 2, 3u),
        0x12345678u
    );


    /* sign_extend tests */
    test_int32(
        "sign_extend -8",
        sign_extend(0xF8u, 8),
        -8
    );

    test_int32(
        "sign_extend most negative 8-bit value",
        sign_extend(0x80u, 8),
        -128
    );

    test_int32(
        "sign_extend positive value",
        sign_extend(0x7Fu, 8),
        127
    );

    test_int32(
        "sign_extend width 32",
        sign_extend(0xFFFFFFFFu, 32),
        -1
    );


    /* status_unpack test 1: required example */
    status_t s1 = status_unpack(0x1631u);

    test_uint32("status 0x1631 heat", s1.heat, 1u);
    test_uint32("status 0x1631 cool", s1.cool, 0u);
    test_uint32("status 0x1631 fan", s1.fan, 0u);
    test_uint32("status 0x1631 fault", s1.fault, 0u);
    test_uint32("status 0x1631 mode", s1.mode, 3u);
    test_uint32("status 0x1631 reserved", s1.reserved, 0u);
    test_int32("status 0x1631 setpoint", s1.setpoint, 22);


    /* status_unpack test 2 */
    status_t s2 = status_unpack(0x0012u);

    test_uint32("status 0x0012 heat", s2.heat, 0u);
    test_uint32("status 0x0012 cool", s2.cool, 1u);
    test_uint32("status 0x0012 fan", s2.fan, 0u);
    test_uint32("status 0x0012 fault", s2.fault, 0u);
    test_uint32("status 0x0012 mode", s2.mode, 1u);
    test_uint32("status 0x0012 reserved", s2.reserved, 0u);
    test_int32("status 0x0012 setpoint", s2.setpoint, 0);


    /* status_unpack test 3 */
    status_t s3 = status_unpack(0xFF0Du);

    test_uint32("status 0xFF0D heat", s3.heat, 1u);
    test_uint32("status 0xFF0D cool", s3.cool, 0u);
    test_uint32("status 0xFF0D fan", s3.fan, 1u);
    test_uint32("status 0xFF0D fault", s3.fault, 1u);
    test_uint32("status 0xFF0D mode", s3.mode, 0u);
    test_uint32("status 0xFF0D reserved", s3.reserved, 0u);
    test_int32("status 0xFF0D setpoint", s3.setpoint, -1);


    printf("\n%d tests run, %d failed\n", tests_run, tests_failed);

    if (tests_failed != 0)
    {
        return 1;
    }

    return 0;
}