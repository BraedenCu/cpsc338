#include "testasm.h"

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  test_sum_diff();
  pinMode (13, OUTPUT);
}


void test_sum_diff() {
  bool test_passed = 1;
  for (int i = 0; i < 100; i++) {
    unsigned char a = random(0, 256);
    unsigned char b = random(0, 256);
    unsigned int sum = a + b;
    unsigned int diff;
    if (a > b) {
      diff = a - b;
    } else {
      diff = b - a;
    }
    unsigned int total = sum+diff;
    if (testasm(a, b) != total) {
      Serial.print("Error: testasm(");
      Serial.print(a);
      Serial.print(", ");
      Serial.print(b);
      Serial.print(") = ");
      Serial.print(testasm(a, b));
      Serial.print(" != ");
      Serial.println(total);
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
