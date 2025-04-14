// ConcurrencySketch.ino

#include "concurrency.h"

// Process function for p1: continuously prints a counter value.
void p1(void) {
  int counter = 0;
  while (1) {
    Serial.println(counter);
    counter++;
    // Optionally, yield here if cooperative scheduling is used:
    // yield();
  }
}

// Process function for p2: continuously toggles the built-in LED.
void p2(void) {
  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);              // delay to visually see the LED on state
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);              // delay to visually see the LED off state
    // Optionally, yield here if cooperative scheduling is used:
    // yield();
  }
}

// Arduino setup: initialize hardware and create processes.
void setup() {
  // Initialize Serial communication once.
  Serial.begin(9600);
  
  // Set LED pin mode once.
  pinMode(LED_BUILTIN, OUTPUT);

  // Create process for p1.
  if (process_create(p1, 64) < 0) {
    Serial.println("Error creating process p1");
    return;
  }
  
  // Create process for p2.
  if (process_create(p2, 64) < 0) {
    Serial.println("Error creating process p2");
    return;
  }
}

// Arduino loop: start the concurrent execution.
void loop() {
  // Start concurrency. This should call process_begin() internally,
  // which uses process_select() to swap between your processes.
  process_start();
  
  // If process_start() returns (which would happen if all processes terminate or a deadlock occurs),
  // print a message and halt.
  Serial.println("All processes finished or deadlock");
  while (1);
}
