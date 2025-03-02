# CPSC 338 lab 1

## Part 1

The directory sketch-part1 contains sketch-part1.ino, testasm.cpp, and testasm.h.

The file testasm.cpp implements the function `unsigned int testasm(unsigned char a, unsigned char b)`. This function adds the 8-bit values `a` and `b` and stores the result in the 16-bit register pair `r25:r24`. The comments in testasm.cpp describe the implementation of this function.

The file sketch-part1.ino contains a test script for this function. It generates 100 random `a`/`b` pairs, adds them, and checks the result against `testasm(a,b)`.

## Part 2

The directory sketch-part2 contains sketch-part2.ino, testasm.cpp, and testasm.h.

The file testasm.cpp implements the function `unsigned int testasm(unsigned char a, unsigned char b)`. This function contains the definitions of two functions in assembly: `unsigned int sumval(unsigned char a, unsigned char b)` and `unsigned int diffval(unsigned char a, unsigned char b)`.
`sumval` is the same as part 1: it adds the 8-bit values `a` and `b` and stores the result in the 16-bit register pair `r25:r24`. `diffval` subtracts the smaller of `a` and `b` from the larger and returns the result in `r25:r24`. 
`testasm` is a wrapper function that returns the `sumval(a,b) + diffval(a,b)` in `r25:r24`. The comments in testasm.cpp describe the implementation of all these functions and how they change and restore the stack.

The file sketch-part2.ino contains a test script for this function. It generates 100 random `a`/`b` pairs, computes the intended result of `testasm(a,b)`, and checks the result against the actual `testasm(a,b)`.

## Part 3

The directory sketch-part3 contains sketch-part3.ino, testasm.cpp, and testasm.h.

The file testasm.cpp implements the function `unsigned int testasm(unsigned char a, unsigned char b)`. This function contains the definitions one functions in assembly: `unsigned int fib(unsigned char n)`.
`fib` computes the nth Fibonacci number and stores the result in `r25:r24`.
`testasm` is a wrapper function that returns `fib(a)` in `r25:r24`. It ignores the value in `b`. The comments in testasm.cpp describe the implementation of these functions and how they change and restore the stack.

The file sketch-part3.ino contains a test script for this function. It compares the result of `testasm(n, 0)` against the nth Fibonacci number for `n` between 0 and 24. The 25th Fibonacci number is too large to fit within 16 bits, so the tests stop at 24.


