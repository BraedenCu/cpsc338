int matrices[10][7][5] = {
  // digit 0
  {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 1, 1},
    {1, 0, 1, 0, 1},
    {1, 1, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0}
  },
  // digit 1
  {
    {0, 0, 1, 0, 0},
    {0, 1, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 1, 1, 1, 0}
  },
  // digit 2
  {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {0, 0, 0, 0, 1},
    {0, 0, 1, 1, 0},
    {0, 1, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1}
  },
  // digit 3
  {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {0, 0, 0, 0, 1},
    {0, 0, 1, 1, 0},
    {0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0}
  },
  // digit 4
  {
    {0, 0, 0, 1, 0},
    {0, 0, 1, 1, 0},
    {0, 1, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 1, 1, 1, 1},
    {0, 0, 0, 1, 0},
    {0, 0, 0, 1, 0}
  },
  // digit 5
  {
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 0},
    {0, 0, 0, 0, 1},
    {0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0}
  },
  // digit 6
  {
    {0, 0, 1, 1, 0},
    {0, 1, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0}
  },
  // digit 7
  {
    {1, 1, 1, 1, 1},
    {0, 0, 0, 0, 1},
    {0, 0, 0, 1, 0},
    {0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0}
  },
  // digit 8
  {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0}
  },
  // digit 9
  {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 1},
    {0, 0, 0, 0, 1},
    {0, 0, 0, 1, 0},
    {0, 1, 1, 0, 0}
  }
};

// Arduino pins that correspond to dot-matrix rows
// These pins need to be high for the LED to turn on
int row_pins[7] = {2, 3, 4, 5, 6, 7, 8};

// Arduino pins that correspond to dot-matrix rows
// These pins need to be low for the LED to turn on
int col_pins[5] = {9, 10, 11, 12, 13};

// Counter for the digit to be displayed
// Last time an intturrupt was triggered
// Debounce delay
int curr_digit = 0;
long last_time = 0;
long debounce_delay = 200;

// Write to a row given the columns that should be turned on in that row
// for example, if row = 0 and cols = {0, 1, 1 1, 0}, then the top row is set to off, on, on, on, off
void write_to_row(int row, int cols[]) {
  // set row pins
  for (int i = 0; i < 7; i++) {
    digitalWrite(row_pins[i], row == i);
  }

  // set column pins
  for (int i = 0; i < 5; i++) {
    digitalWrite(col_pins[i], !cols[i]);
    delay(1);
    digitalWrite(col_pins[i], 1);
  }

  // turn off the row pin
  digitalWrite(row_pins[row], 0);
}

// Check if the button was pressed and inturrupt sevice routine was triggered
void button_interrupt() {
    curr_time = millis();

    if (curr_time - last_time > debounce_delay) {
        // update current digit, ensure that it wraps around 0-9
        curr_digit = (curr_digit + 1) % 10;
        last_time = curr_time;
    }
}


void display_digit(int digit) {
  // display the digit
  Serial.println(digit);
  for (int i = 0; i < 7; i++) {
    write_to_row(i, matrices[digit][i]);
  }
}

void setup() {
  Serial.begin(9600);

  // set all row pins to output
  for (int i = 0; i < 7; i++) {
    pinMode(row_pins[i], OUTPUT);
  }

  // set all column pins to output
  for (int i = 0; i < 5; i++) {
    pinMode(col_pins[i], OUTPUT);
  }
  
  // Set pin2 as input pullup
  pinMode(2, INPUT_PULLUP);

  // Attach inturrupt to PD2
  attachInterrupt(digitalPinToInterrupt(2), change_digit, FALLING); 
}


void loop() {
    /*
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 50; j++) {
      display_digit(i);
      // int a[5] = {1,0,0,0,1};
      // write_to_row(1, a);
    }
  }
    */
    display_digit(curr_digit);
    delay(1);
}
