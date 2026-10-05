This homework is about learning how to work with bits in C. The main parts of the homework were making functions that can print bits, read certain bits from a number, replace certain bits in a number, sign extend a number, and then use those functions to decode a thermostat status word.

One of the biggest things I learned was how bit shifting and masks work. For example, in get_field() I first shift the word to the right by the starting position. This moves the bits I want down toward bit 0. After that I make a mask using (1u << width) - 1u. The mask creates a group of 1s that is the same size as the width. I then use bitwise AND with the mask so all the unwanted bits are removed and only the field I want is left.

For example, get_field(word, 2, 3) means start at bit 2 and take 3 bits total. This means the function uses bits 2, 3, and 4. At first I was confused because binary numbers are usually written with the higher numbered bits on the left, so the same field can also look like bits 4, 3, and 2 when looking at the number from left to right.

I also had to handle special cases in get_field(). The width must be between 1 and 32, the starting position must be between 0 and 31, and the position plus width cannot go past 32 bits. If the input is invalid, my function returns 0. I also had to handle the case where the width is exactly 32 separately because shifting a 32-bit value by 32 is not valid in C.

The set_field() function works in a similar way, except instead of reading bits, it replaces bits. I first create a mask for the width and then shift that mask to the correct position. I use word & ~shifted_mask to clear the old bits. The `~` flips the mask so the field I want to replace becomes zero while the other bits stay unchanged. Then I trim the new value using value & mask so the value cannot use more bits than the field allows. After that I shift the trimmed value into the correct position and combine it with the cleared word using bitwise OR.

If the inputs to set_field() are invalid, I return the original word unchanged. This made the most sense to me because if the requested field is invalid, the function should just leave the word alone. If the width is 32, then the entire word is being replaced, so I return the new value.

The print_binary() function prints a value in binary. It uses a loop that starts at width - 1 and counts down to 0 using i--. I learned that i-- means subtract 1 from `i` each time through the loop. The expression (x >> i) & 1u moves the current bit to bit 0 and then uses bitwise AND with 1 to keep only that bit. The function also prints a space every 4 bits to make the binary easier to read. For example, print_binary(0x2C, 8) prints 0010 1100. If the width is invalid, the function uses return; because the function has a void return type and does not return a value.

The sign_extend() function was probably the most confusing part at first. Its purpose is to take a smaller signed value and turn it into a proper 32-bit signed value. For example, 0xF8 as an 8-bit signed number represents -8. The function finds the sign bit using 1u << (width - 1). If that sign bit is 1, the number is negative. I then fill the upper bits with 1s using value | ~mask. This keeps the same negative value after it becomes a 32-bit signed integer. If the sign bit is 0, the value is positive and does not need the upper bits filled with 1s.

I also learned the difference between bitwise operators and logical operators. & is bitwise AND and works directly on bits. | is bitwise OR and also works directly on bits. && is logical AND and is mainly used to combine conditions, and || is logical OR. For example, width < 1 || width > 32 means the width is invalid if either condition is true.

The thermostat part of the homework uses the bit functions to decode a 16-bit status word. Bit 0 represents heat, bit 1 represents cool, bit 2 represents fan, bit 3 represents fault, bits 4 through 6 represent the mode, bit 7 is reserved, and bits 8 through 15 represent the setpoint. I used get_field() to extract each field from the status word. The setpoint is signed, so after getting bits 8 through 15 I pass that value through sign_extend().

For example, status_unpack(0x1631) gives a setpoint of 22, a mode of 3, no fault, no fan, no cooling, and heat turned on. This matched the expected result from the assignment.

I also ran into several syntax errors while working on the homework. One issue was accidentally putting a semicolon after a function header. For example, writing void print_binary(uint32_t x, int width); is only a function declaration, not the actual function definition. I also accidentally used -shifted_mask when I needed ~shifted_mask. I had a few misspelled variable names and also forgot an #endif in one of the header files. Fixing these errors helped me understand how function definitions, header guards, and bitwise operators work better.

For testing, I tested normal cases along with edge cases. I tested a width of 1, a width of 32, position 31, a value that was too wide for its field, positive and negative sign extension, the most negative 8-bit signed value, and multiple thermostat status words. I also tested the required 0x1631 thermostat example.

The thermostat mode field is stored in the status.mode. Modes zero through 4 are valid. If status.mode contains 5, 6, or 7, then it shows as an invalid thermostat mode. The reserved bit is stored in status.reserved and should normally be 0.

To compile the homework I can use:

make

To run the tests I can use:

make test

To remove the compiled object files and executable I can use:

make clean