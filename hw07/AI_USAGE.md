# AI_USAGE.md

I used ChatGPT to help with Homework 7, mainly for understanding dynamic memory, and checking for possible errors.

One thing I needed help understanding was how realloc works when the stack becomes full. I learned that assigning realloc directly to the original pointer can cause problems if the allocation fails. ChatGPT explained why using a temporary pointer is safer.

Another part I worked on was handling NULL pointers and empty stacks. I wanted to make sure that functions like istack_pop and istack_peek would not crash when there were no values in the stack. AI helped explain what each function should return in those situations.

I also used ChatGPT to help build and check the bracket checker. Some important things were keeping track of line and column numbers, detecting mismatched brackets, and reporting when brackets were still open at the end of the input.

