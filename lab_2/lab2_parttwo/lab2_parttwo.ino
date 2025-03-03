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

// Write to a row given the columns that should be turned on in that row
// for example, if row = 0 and cols = {0, 1, 1 1, 0}, then the top row is set to off, on, on, on, off
void write_to_row(int row, int cols[]) {
  // set row pins
  for (int i = 0; i < 7; i++) {
    digitalWrite(row_pins[i], row == i);
  }

  // Serial.print("Printing: ");
  // set column pins
  for (int i = 0; i < 5; i++) {
    digitalWrite(col_pins[i], !cols[i]);
    // Serial.print(cols[i]);
  }
  // Serial.print(" to row ");
  // Serial.println(row);


}


void display_digit(int digit) {
  // display the digit
  Serial.println(digit);
  for (int i = 0; i < 7; i++) {
    write_to_row(i, matrices[digit][i]);
    delay(1);
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
}


void loop() {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 50; j++) {
      display_digit(i);
    }
  }
}
