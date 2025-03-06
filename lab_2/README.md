# Lab 2

## Part 1: Digital input by polling
This section just involved debouncing a button. The button is connected to the Arduino as shown in the part1_circuit.pdf. The code debounces the button by reading three inputs successively. If two of the three inputs say the button is pressed, the software considers the button to be pressed.
 
# Part 2
this section involved controlling an LED matrix using two 595 shift registers. The circuit is shown in part2_circuit.pdf*. The code writes to the shift registers using three pins: SER, RCLK, and SRCLK on the shift register. Using these pins, it writes 12 bits to the registers using serial communication. The 12 bits are output to the matrix in parallel. The matrix displays the numbers 0-9 in a cycle.

# Part 3
Part 3 combines part 2 and part 1 in which a button is used to cycle through the numbers. The circuit is shown in part3_circuit.pdf*. This part uses an interrupt service routine to change the digit displayed on the LED matrix when the button is pressed.


*In the pdfs, the LED matrix is displayed as a dip-switch because the software we used to create the diagrams did not have an LED matrix. The top-right pin is pin one on the LED matrix.
