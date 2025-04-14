// sketch_part1.ino

#include "concurrency.h"
#include <Arduino.h>

void p1(void) {
  int counter = 0;
  while (1) {
    char character = counter + 97;
    cli();
    Serial.println(character);
    sei();
    counter = (counter + 1) % 26;
  }
}

void p2(void) {
  int counter2 = 0;
  while (1) {
    cli();
    Serial.println(counter2);
    sei();
    counter2++;
  }
}

void p3(void) {
  long counter = 1000;
  while (1) {
    cli();
    Serial.print("p3: ");
    Serial.println(counter);
    sei();
    counter += 1000;
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  if (process_create(p1, 64) < 0) {
    Serial.println("Error creating process p1");
    return;
  }

  if (process_create(p2, 64) < 0) {
    Serial.println("Error creating process p2");
    return;
  }

  // testing with a third process
  if (process_create(p3, 64) < 0) {
    Serial.println("Error creating process p3");
    return;
  }
}

void loop() {
  process_start();

  // if we get here it means all processes ended or deadlocked
  Serial.println("All processes finished or deadlock");
  while (1);
}
