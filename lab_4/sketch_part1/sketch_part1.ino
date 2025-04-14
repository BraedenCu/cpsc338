// sketch_part1.ino

#include "concurrency.h"
#include <Arduino.h>

void p1(void) {
  int counter = 0;
  while (1) {
    cli();
    Serial.println(counter);
    sei();
    counter++;
    // yield();  // switch to p2
  }
}

void p2(void) {
  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    // yield();  // switch to p1
  }
}

void p3(void) {
  long counter = 1000;
  while (1) {
    Serial.print("p3: ");
    Serial.println(counter);
    counter += 1000;
    yield(); // cooperative yielding
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
  // if (process_create(p3, 64) < 0) {
  //   Serial.println("Error creating process p3");
  //   return;
  // }
}

void loop() {
  process_start();

  // if we get here it means all processes ended or deadlocked
  Serial.println("All processes finished or deadlock");
  while (1);
}
