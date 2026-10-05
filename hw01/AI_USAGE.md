I used ChatGPT to help me understand the homework requirements and learn how the bit manipulation functions work.

I used AI help to better understand:
- how get_field() uses shifting and masking
- how set_field() clears and replaces bits
- the difference between &, &&, |, and ||
- how i-- works in a loop
- how sign_extend() works
- why width == 32 needs to be handled separately
- how to debug compiler errors
- how to create and run tests
- how to make and use a Makefile

One thing I had to fix from the AI-generated help was the thermostat status structure. At first the reserved field was left out and status_unpack used numbers directly for the bit positions and widths. I found this by comparing the code back to the homework instructions, which said status_t needs one member per field and named constants must be used for every position and width. I then added the reserved field and the named constants.

I also used ChatGPT to help identify syntax errors in my code, such as missing semicolons, incorrect braces, misspelled variable names, and incorrect operators.
I wrote and reviewed the code while working through the explanations so that I could understand what each part was doing.