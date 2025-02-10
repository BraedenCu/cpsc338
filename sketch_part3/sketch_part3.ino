#include "testasm.h"

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);

  test_fib();

  pinMode (13, OUTPUT);
}

void test_fib() {
  int valid_fib[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368};
  bool test_passed = 1;

  for (int i = 0; i < sizeof(valid_fib) / sizeof(int); i++) {
    if (testasm(i , 0) != valid_fib[i]) {
      Serial.print("Error: ");
      Serial.print(i);
      Serial.print(" = ");
      Serial.print(testasm(i, 0));
      Serial.print(" != ");
      Serial.println(valid_fib[i]);
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
