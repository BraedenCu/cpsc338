#include "testasm.h"

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  test_sum();
  pinMode (13, OUTPUT);
}

void test_sum() {
  bool test_passed = 1;
  for (int i = 0; i < 100; i++) {
    unsigned char a = random(0, 256);
    unsigned char b = random(0, 256);
    unsigned int sum = a + b;
    if (testasm(a, b) != sum) {
      Serial.print("Error: ");
      Serial.print(a);
      Serial.print(" + ");
      Serial.print(b);
      Serial.print(" = ");
      Serial.print(testasm(a, b));
      Serial.print(" != ");
      Serial.println(sum);
      test_passed = 0;
    }
  }
  if (test_passed) {
    Serial.println("Tests passed!");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (13, HIGH);
  delay(400);
  digitalWrite (13, LOW);
  delay(400);
}
