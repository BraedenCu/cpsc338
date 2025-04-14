// sketch_part1.ino

#include "concurrency.h"
#include <Arduino.h>

void p1(void) {
  int counter = 0;
  while (1) {
    Serial.println(counter);
    counter++;
    yield();  // Let the scheduler switch to p2
  }
}

void p2(void) {
  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    yield();  // Also yield here
  }
}

// Third process: prints a bigger counter
void p3(void) {
  long counter = 1000;
  while (1) {
    Serial.print("p3: ");
    Serial.println(counter);
    counter += 1000;
    // If cooperative scheduling, you can do: yield();
  }
}

void setup() {
  // Initialize Serial
  Serial.begin(9600);

  // Initialize LED
  pinMode(LED_BUILTIN, OUTPUT);

  // Debug message
  Serial.println("Initialize serial port");

  // Create process p1
  if (process_create(p1, 64) < 0) {
    Serial.println("Error creating process p1");
    return;
  }

  // Create process p2
  if (process_create(p2, 64) < 0) {
    Serial.println("Error creating process p2");
    return;
  }

  // if (process_create(p3, 64) < 0) {
  //   Serial.println("Error creating process p3");
  //   return;
  // }
}

void loop() {
  // Begin the concurrency system.
  process_start();

  // If we ever return here, it means all processes ended or deadlocked.
  Serial.println("All processes finished or deadlock");
  while (1);
}
