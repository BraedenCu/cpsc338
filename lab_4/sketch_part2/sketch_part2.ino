// sketch_part1.ino

#include "concurrency.h"
#include <Arduino.h>


lock_t *l;  // our shared lock

const int RED_PIN   = 9;
const int GREEN_PIN = 10;
const int BLUE_PIN  = 11;

// P1: green blink sequence
void p1(void) {
  while (1) {
    lock_acquire(l);
      Serial.println("=== P1 BEGIN ===");
      for (int i = 0; i < 5; i++) {
        digitalWrite(GREEN_PIN, HIGH);
        delay(200);
        digitalWrite(GREEN_PIN, LOW);
        delay(200);
      }
      Serial.println("=== P1 END   ===");
    lock_release(l);
    yield();
    delay(500);
  }
}

// P2: red blink sequence
void p2(void) {
  while (1) {
    lock_acquire(l);
      Serial.println("+++ P2 BEGIN +++");
      for (int i = 0; i < 5; i++) {
        digitalWrite(RED_PIN, HIGH);
        delay(200);
        digitalWrite(RED_PIN, LOW);
        delay(200);
      }
      Serial.println("+++ P2 END   +++");
    lock_release(l);
    yield();
    delay(500);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN,   OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN,  OUTPUT);

  // allocate and init lock
  cli();
    l = process_malloc(sizeof(lock_t));
  sei();
  lock_init(l);

  // register processes
  if (process_create(p1, 64) < 0) Serial.println("Error creating p1");
  if (process_create(p2, 64) < 0) Serial.println("Error creating p2");
}

void loop() {
  process_start();
  // if we ever return:
  Serial.println("!!! deadlock or all done !!!");
  while (1);
}
