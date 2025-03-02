# Lab 2: Digital I/O

Due: Mar 7, 11:59pm. Submit on Canvas.
Weight: 10%

Goal. The goal of this lab is is to use the AVR microcontroller to implement digital I/O, and become familiar with the data sheets of key components in your kits. The digital inputs we will use are push-button switches, and the digital outputs we will use are LEDs. To complete this lab, you will need to understand digital I/O concepts covered in class in addition to basic Arduino skills from Lab 1.

Datasheets for all components in the kit are availableLinks to an external site.. You won't be using assembly language for this lab; everything should be implemented using C/C++ in the Arduino environment. We are staging a more advanced workshop that need more help coming up to speed on the Arduino environment.

  Part 1: Digital input by polling
The goal of this part of the lab is to implement a polling-based approach to reading the state of a switch.

Use the jumper wires, breadboard, and push-button switch to build a circuit that is connected to the PD2 pin on the ATmega328P.
Use the PORTD, PIND, and DDRD I/O mapped registers to configure the pin as an input with a passive pull-up. (Note: the I/O mapped registers are conveniently mapped to variables you can directly access.)
Write code that displays the current state of the switch to the terminal, printing something only when the state of the switch changes.
The program must include code that de-bounces the switch.
For this part, do not use the Arduino digitalRead()/pinMode() functions. You can use these functions if you want to debug your circuit on the breadboard, but the final implementation should not use these functions. (Note: the implication here is that you are to read off-board values through the SFR registers that map to these port locations.)

 

  Part 2: Digital output: dot-matrix LED display
The goal of this lab is to implement digital output using the dot-matrix LED display. For this part, you must use the I/O mapped registers to configure the I/O pins and drive the outputs. Once again, do not use the digitalWrite()/pinMode() functions from the Arduino package (you can use them for debugging, but not for your submission).

Use the jumper wires, breadboard, 330 ohm resistors, two shift registers, and 5x7 LED matrix to wire up the LED matrix to some of the digital outputs on the Uno board (you can use outputs 4 to 12).
The two shift registers can be used to extend the number of digital output pins. There is a nice tutorial on how you can do this onlineLinks to an external site.. Connect the two registers in a chained pattern.
Make sure to use current limiting resistors (we have given you ten) when using the dot matrix LED display.
Write a program that cycles through the digits 0 to 9 on the dot matrix display.
An individual LED in the display can be set by applying the appropriate binary pattern to the pins of the 5x7 LED matrix.
Cycling through different binary patterns will give the illusion of all the appropriate LEDs on the matrix being simultaneously lit.
There are many "fonts" available onlineLinks to an external site. that specify LED patterns for digits, in case you don't want to create your own.
 

  Part 3: Counter and interrupts
In this part, we will combine your Circuit from Part 2 with the use of Interrupts. Connect the push-button to PD2, as in Part 1. Instead of polling, you should read the state of the switch using interrupts. You will still need to de-bounce the switch state as before, but this should be done via interrupts and not by polling. For this part, you can use the Arduino pinMode()/digitalWrite()/etc. functions if you prefer.

Normally we would set interrupt handlers/etc. using the syntax provided by the C/C++ compiler. However, the Arduino library that your program links against already has interrupt handlers defined. Details of how the interrupt handlers are defined, as well as the implementation of attachInterrupt() and detatchInterrupt() can be found on githubLinks to an external site..

Use the Arduino attachInterrupt()Links to an external site. function to install your interrupt handler. (Note: See video tutorial in class about using such Interrupt processing).

The display should start with digit 0. Pushing the button should increment the count shown on the display, wrapping around from 9 to 0.

  What you have to submit
You should turn in three sketches, one for each part of the lab, as a single zip file. A simple way to organize this would be to have three sketches, "sketch_part1", "sketch_part2", "sketch_part3". Please make sure your program is well-commented. In addition, for each part, provide a PDF document that includes the circuit schematic that you used for each part. If you want to use software to create a pretty schematic, there's a free software package called FritzingLinks to an external site. that can be used for this purpose, although a hand-drawn and scanned schematic is fine. (The Fritzing developers are asking for contributions to download the latest version; you can also build it yourself from GithubLinks to an external site., or download an older binaryLinks to an external site. for free.) Please upload one zip file per group.

For this lab, we would to see a demo of Part 3 to represent the completion of this Lab. Create a short video that shows Part 3 working, and include it as part of your submission. Include a plain text file called "demo.txt" that contains enough information so we can view your demo video (e.g. a URL). If the file isn't huge, you could even include the video as part of your submission.