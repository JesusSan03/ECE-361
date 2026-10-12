
ECE 361 - Homework 2 AI Usage

AI Tool Used - ChatGPT - How I Used AI

I used ChatGPT to help me understand some parts of the assignment and to help with writing and organizing the code. I also used it to explain concepts that I was confused about and help with errors while working on the homework.

Problems and Misunderstandings

One issue I had was understanding the difference between header files and source files. I was confused about why functions needed to be declared in .h files when the actual code was already written in the .c files. I learned that header files let other parts of the program know which functions they can use.

Another thing I had trouble understanding was how recursion worked in the maximum function. I was confused about how the function could call itself and still return the correct value. I learned that it keeps calling itself with smaller parts of the array until it reaches the base case.

One mistake in the AI-generated code was that the unit tests originally used global variables to count passed and failed tests. This did not follow the assignment requirements, so I changed the counters to local variables and passed their addresses to the test function.

I was also confused about how the Makefile knew which files to compile. I learned that the dependencies tell Make which files need to be compiled again when something changes.

Testing

I used the terminal to compile the program and run the tests. I checked the compiler output and the test results to make sure everything worked correctly. The unit tests and regression tests passed.
