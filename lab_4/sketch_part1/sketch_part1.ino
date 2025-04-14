#include "concurrency.h"

void p1 (void) {
  int counter = 0;
  while (1) {
    Serial.println(counter);
    counter++;
  }
  return;
}

void p2 (void) {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  int led_val = 0;

  while (1) {
    digitalWrite(LED_BUILTIN, led_val);
    led_val = !led_val;
  }
  return;
}

void setup()
{
  if (process_create (p1, 64) < 0) {
    return;
  }
  if (process_create (p2, 64) < 0) {
    return;
  }
}

void loop()
{
  process_start();
  /* if you get here, then all processes are either finished or
     there is deadlock */
  Serial.println("all processes finished or deadlock");
  while (1) ;
}
