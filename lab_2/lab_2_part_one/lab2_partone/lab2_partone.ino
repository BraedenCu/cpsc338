void setup() {
  // setup button code
  pinMode(2, INPUT);
  Serial.begin(9600);  
}

void loop() {
  // sample every few milliseconds, take five readings and if three out of the five are registerd as high, then the button shoul be pressed
  int buttonState = 0;
  // begin sampling
  int* stored_samples = new int[5];
  for (int i = 0; i < 5; i++) {
    stored_samples[i] = digitalRead(2);
    delay(100);
  }
  // count the number of high readings
  int high_count = 0;
  for (int i = 0; i < 5; i++) {
    if (stored_samples[i] == HIGH) {
      high_count++;
    }
  }

  // if three out of the five readings are high, then the button is pressed
  if (high_count >= 3) {
    buttonState = HIGH;
  } else {
    buttonState = LOW;
  }

  // print the button state
  Serial.println(buttonState);
}
