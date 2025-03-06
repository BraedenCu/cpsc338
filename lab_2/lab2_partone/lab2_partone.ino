uint8_t button_pin = 2;
int delay_amt = 10;

void setup() {
  DDRD &= ~(1 << button_pin); // Set button_pin to input
  PORTD |= (1 << button_pin); // Set button_pin to passive pull-up

  Serial.begin(9600);  
}

int debounce(int pin) {
  // begin sampling
  int count = 0;
  // iterate three times, and capture three samples for debouncing
  for (int i = 0; i < 3; i++) {
    // if the pin is high, increment count
    count += (PIND >> pin) & 1;
    // delay for 10ms
    delay(delay_amt);
  }

  // if 2 out of the 3 readings are high, then the button is pressed
  if (count >= 2) {
    return HIGH;
  } else {
    return LOW;
  }
}

int prev = 0;

void loop() {
  // debounce the button, determine state
  int curr = debounce(button_pin);

  // print based on state
  if (curr != prev) {
    if (curr == HIGH) {
      Serial.println("Button not pressed");
    } else {
      Serial.println("Button pressed");
    }
  }

  prev = curr;
}
