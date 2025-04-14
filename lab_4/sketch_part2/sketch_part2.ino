// sketch_part1.ino

#include "concurrency.h"
#include <Arduino.h>

lock_t* l = NULL;

void p1(void) {
  int last_blink1 = millis();
  while (1) {
    int curr_time1 = millis();
    if ( curr_time1 >= (last_blink1 + 3000) && !l->is_taken) {
      lock_acquire(l);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      lock_release(l);
      last_blink1 = curr_time1;
    }
  }
}

void p2(void) {
  int last_blink2 = millis();
  while (1) {
    int curr_time2 = millis();
    if ( curr_time2 >= (last_blink2 + 4500) && !l->is_taken) {
      lock_acquire(l);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(20);
      digitalWrite(LED_BUILTIN, LOW);
      lock_release(l);
      last_blink2 = curr_time2;
    }
  }
}

// void p3(void) {
//   long counter = 1000;
//   while (1) {
//     cli();
//     Serial.print("p3: ");
//     Serial.println(counter);
//     sei();
//     counter += 1000;
//   }
// }

void setup() {
  cli();
  l = process_malloc(sizeof(lock_t));
  lock_init(l);
  sei();
  // Serial.begin(9600);

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
