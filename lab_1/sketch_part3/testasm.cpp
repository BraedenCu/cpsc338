#include "testasm.h"

/* the attribute "used" tells the compiler to not inline this function */
__attribute__((used))
unsigned int testasm(unsigned char a, unsigned char b) {
  unsigned int out;

  asm volatile (R"(

   rjmp start_of_assembly

   .data
   ; any data values/static storage can go here

   .text

start_of_assembly:
   ; Your assembly language program goes here
   ;
   ; For this example, your program must not modify any registers other than 
   ; r24 and r25. In other words, save and restore all the registers that
   ; are modified by your code.

   ; Tell the compiler to move the arguments a, b into registers 
   ; r24, r25 respectively
   ;
   mov r24,%1
   mov r25,%2
   ;
   ;  --- YOUR CODE GOES HERE ---
   ;       r24 = a, r25 = b
   ;

    ; Only use the value in a (the testasm function just calls fib(a))
   ldi r25, 0x00
   call fib
   rjmp end_of_assembly

  ; definition of 
  ; unsigned int fib(unsigned char n)
  fib:

    ; if (n == 0)
    cpi r24, 0x00
    breq zero

    ; if (n == 1)
    cpi r24, 0x01
    breq one

    ; we use r0 and r1 in this function, so push their current values onto the stack
    push r0
    push r1

    ; decrement r24 and push its value (n-1) to the stack
    dec r24
    push r24

    ; call fib(n-1) and move the result to r1:r0
    call fib
    movw r0, r24

    ; pop (n-1) from the stack to r24
    pop r24

    ; call fib(n-2)
    dec r24
    call fib
    
    ; add fib(n-1) with fib(n-2), and store result in r25:r24
    add r24, r0
    adc r25, r1

    ; restore r1 and r0 to their original values
    pop r1
    pop r0

    ret

  ; return 0
  zero: 
    ldi r24, 0x00
    ldi r25, 0x00
    ret

  ; return 1
  one:
    ldi r24, 0x01
    ldi r25, 0x00
    ret

   ;
   ;  --- YOUR CODE ENDS ---
   ;      The result must be in the register pair r25:r24
   ;      You can also rjmp to the label end_of_assembly if you need to.
   ;
end_of_assembly: 
   ; -- move r25:r24 to the 16-bit word in variable out
   movw %0,r24

)" : "=w" (out)  /* out is %0, and a 16-bit variable that is modified
		    by the assembly language */
   : "r" (a), "r" (b)  /* a is %1, b is %2, both are register-sized
			  variables */
   : "r25", "r24");   /* r24, r25 are modified by this assembly
			 language fragment */

  return out;
}
