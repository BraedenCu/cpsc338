// sketch_part1.ino

#include "concurrency.h"
#include <Arduino.h>

const int RED_PIN_LED = 9;
const int GREEN_PIN_LED = 10;
const int BLUE_PIN_LED = 11;

int DISPLAY_TIME = 100; 

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

void p4(void) {
  unsigned long last_blink2 = millis();
  while (1) {
    unsigned long curr_time2 = millis();
    if ( curr_time2 >= (last_blink2 + 1000)) 
    {
      for(int i = 0; i < 5; i+=1) {
        //digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(RED_PIN_LED, LOW);
        digitalWrite(GREEN_PIN_LED, LOW);
        digitalWrite(BLUE_PIN_LED, HIGH);
        delay(50);
        //digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(RED_PIN_LED, LOW);
        digitalWrite(GREEN_PIN_LED, LOW);
        digitalWrite(BLUE_PIN_LED, LOW);
        delay(50);
      }
      last_blink2 = curr_time2;
    }
  }
}

void p5(void) {
  unsigned long last_blink2 = millis();
  while (1) {
    unsigned long curr_time2 = millis();
    if ( curr_time2 >= (last_blink2 + 20)) 
    {
      for(int i = 0; i < 10; i+=1) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(50);
        digitalWrite(LED_BUILTIN, LOW);
        delay(50);
      }
      last_blink2 = curr_time2;
    }
  }
}

/************ TEST ONE *************/
void test_one_serial_led() {

  if (process_create(p1, 64) < 0) {
    Serial.println("Error creating process p3");
    return;
  }

  if (process_create(p5, 64) < 0) {
    Serial.println("Error creating process p3");
    return;
  }

  if (process_create(p4, 64) < 0) {
    Serial.println("Error creating process p3");
    return;
  }
}

/************ TEST TWO *************/
void test_two_serial_only() {

  if (process_create(p1, 64) < 0) {
    Serial.println("Error creating process p3");
    return;
  }

  if (process_create(p2, 64) < 0) {
    Serial.println("Error creating process p3");
    return;
  }

  if (process_create(p3, 64) < 0) {
    Serial.println("Error creating process p3");
    return;
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  /************ TEST ONE *************/
  //test_one_serial_led();

  /************ TEST TWO *************/
  test_two_serial_only();
}

void loop() {
  process_start();

  // if we get here it means all processes ended or deadlocked
  Serial.println("All processes finished or deadlock");
  while (1);
}
