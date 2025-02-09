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

    ; this function uses r0 and r1, so push them to the stack
    push r0
    push r1

    ; push a and b to the stack
    push r24
    push r25

    ; call sumval(a, b) and move the result to r1:r0
    call sumval
    mov r0, r24
    mov r1, r25

    ; pop a and b back to r24 and r25
    pop r25
    pop r24

    ; call diffval(a, b)
    call diffval

    ; set r25:r24 <- r25:r24 + r1:r0
    add r24, r0
    adc r25, r1

    ; replace r1 and r0 with their original values
    pop r1
    pop r0

    ; exit testasm
    rjmp end_of_assembly


  ; Definition of sumval (same as part 1)
  sumval:

    ; r24 <- r24 + r25
    add r24, r25

    ; set r25 to 0
    ldi r25, 0x00

    ; if the carry is set, branch to set_carry
    brcs set_carry
    ; otherwise, return
    ret

  ; set r25 to 1 and return
  set_carry:
    ldi r25, 0x01
    ret

  diffval:

    ; compare r25 with r24
    cp r25, r24

    ; branch to lessthan if r25 is less than r24
    brlo lessthan

    ; otherwise, set r24 <- r25 - r24 (b-a), set r25 to 0, and return
    sub r25, r24
    mov r24, r25
    ldi r25, 0x00
    ret

    ; set r24 <- r24 - r25 (a-b), set r25 to 0, and return
  lessthan:
    sub r24, r25
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
