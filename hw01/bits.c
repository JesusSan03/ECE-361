#include <stdio.h>
#include <stdint.h>
#include "bits.h"

void print_binary(uint32_t x, int width)
{
    if (width < 1 || width > 32)
    {
        return;
    }

    for (int i = width - 1; i >= 0; i--)
    /* matches the # of bits to the width*/
    {
        printf("%u", (x >> i) & 1u);
        /*moves i to bit zero then Ands it with 1u, so if i = 1, then its 1,
        if i = 0 then its 0 */

        if (i % 4 == 0 && i != 0)
        {
            printf(" ");
            /* every 4 bits it adds a space*/
        }
    }

    printf("\n");
}


uint32_t get_field(uint32_t word, int pos, int width)
{
    if (width < 1 || width > 32)
    /* The width must be in between 1 and 32*/
    {
        return 0;
    }

    if (pos < 0 || pos > 31)
    /* The starting position needs to be a real position in a 32-bit value*/
    {
        return 0;
    }

    if (pos + width > 32)
    /* The starting position # and ending position # must not be greater than 32*/
    {
        return 0;
    }

    if (width == 32)
    {
        return word;
    }

    uint32_t shifting = word >> pos;
    /*Shift the desired bits to the right beginning at bit 0, this happens
    because we are using our pos to shift the entire word by that amount*/
    /* We now must remove the unwanted bits, i think we can do that opposite
    by using width now*/

    uint32_t mask = (1u << width) - 1u;
    /*This creates a mask of 1's that matches the quantity length as the width*/

    uint32_t anded = shifting & mask;
    /* first, we move the desired bits to the right, second we get a mask that
    is the same length as our desired bits, then finally we & And them together
    to remove all other unwanted bits*/

    return anded;
}


uint32_t set_field(uint32_t word, int pos, int width, uint32_t value)
{
    if (width < 1 || width > 32)
    /* The width must be in between 1 and 32*/
    {
        return word;
    }

    if (pos < 0 || pos > 31)
    /* The starting position needs to be a real position in a 32-bit value*/
    {
        return word;
    }

    if (pos + width > 32)
    /* The starting position # and ending position # must not be greater than 32*/
    {
        return word;
    }

    if (width == 32)
    {
        return value;
    }

    uint32_t mask = (1u << width) - 1u;
    /*This creates a mask of 1's that matches the quantity length as the width*/

    uint32_t shifted_mask = mask << pos;
    /* Mask now lines up to the three bits we want to replace*/

    uint32_t cleared = word & ~shifted_mask;
    /* the tilda ~ flips every bit so its essentially inverts the mask,
    and by ANDing that inverted mask we delete all of those bits*/

    uint32_t trimmed_value = value & mask;
    /* The value is trimmed with the mask so it does not exceed the width*/

    uint32_t shifted_value = trimmed_value << pos;
    /* The trimmed value is now moved into our desired position*/

    uint32_t result = cleared | shifted_value;
    /* The cleared word is now ORed with the shifted new value*/

    return result;
}


int32_t sign_extend(uint32_t value, int width)
{
    if (width < 1 || width > 32)
    {
        return 0;
    }

    if (width == 32)
    {
        return (int32_t)value;
    }

    uint32_t sign_bit = 1u << (width - 1);

    uint32_t mask = (1u << width) - 1u;

    value = value & mask;

    if (value & sign_bit)
    {
        value = value | ~mask;
    }

    return (int32_t)value;
}