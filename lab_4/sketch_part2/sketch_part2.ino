// sketch_part1.ino

#include "concurrency.h"
#include <Arduino.h>

lock_t* l = NULL;

const int RED_PIN_LED = 9;
const int GREEN_PIN_LED = 10;
const int BLUE_PIN_LED = 11;

int DISPLAY_TIME = 100; 

void p1(void) {
  unsigned long last_blink1 = millis();
  while (1) {
    unsigned long curr_time1 = millis();
    if ( curr_time1 >= (last_blink1 + 1000) && !l->is_taken) {
      lock_acquire(l);
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(RED_PIN_LED, LOW);
      digitalWrite(GREEN_PIN_LED, HIGH);
      digitalWrite(BLUE_PIN_LED, LOW);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(RED_PIN_LED, LOW);
      digitalWrite(GREEN_PIN_LED, LOW);
      digitalWrite(BLUE_PIN_LED, LOW);
      lock_release(l);
      last_blink1 = curr_time1;
    }
  }
}

void p2(void) {
  unsigned long last_blink2 = millis();
  while (1) {
    unsigned long curr_time2 = millis();
    if ( curr_time2 >= (last_blink2 + 4500) && !l->is_taken) {
      
      lock_acquire(l);
      for(int i = 0; i < 10; i+=1) {
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(RED_PIN_LED, HIGH);
        digitalWrite(GREEN_PIN_LED, LOW);
        digitalWrite(BLUE_PIN_LED, LOW);
        delay(20);
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(RED_PIN_LED, LOW);
        digitalWrite(GREEN_PIN_LED, LOW);
        digitalWrite(BLUE_PIN_LED, LOW);
        delay(20);
      }
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

  pinMode(RED_PIN_LED, OUTPUT);
  pinMode(GREEN_PIN_LED, OUTPUT);
  pinMode(BLUE_PIN_LED, OUTPUT);

  digitalWrite(RED_PIN_LED, LOW);
  digitalWrite(GREEN_PIN_LED, LOW);
  digitalWrite(BLUE_PIN_LED, LOW);

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
