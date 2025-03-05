// Font definition 
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

// define pin values
#define SER 4
#define RCLK 5
#define SRCLK 6

// Basically the same as digitalWrite
// Write value (1 or 0) to an output pin
// pin must be initialized to output for this to work
void write_to_pin(int pin, int value) {
  if (value) {
    PORTD |= (1 << pin);
  } else {
    PORTD &= ~(1 << pin);
  }
}

// Set an LED matrix row to the values in cols
// for example, if row = 0 and cols = {0, 1, 1, 1, 0}, then the top row is set to off, on, on, on, off
//                                                     and the rest of the rows are turned off
void write_to_row(int row, int cols[]) {
  // Set RCLK pin on shift register to 0
  // this way, it does not update the register output pins until RCLK goes high
  write_to_pin(RCLK, 0);

  // Shift all of the row values
  // Only want to turn on the matrix row corresponding to the variable row
  // A row needs to be set to 1 for the LEDs to turn on
  for (int i = 0; i < 7; i++) {
    write_to_pin(SRCLK, 0);
    write_to_pin(SER, i == row);
    write_to_pin(SRCLK, 1);
  }

  // Shift all of the column values
  // The column needs to be set to 0 for the LED to turn on
  for (int i = 0; i < 5; i++) {
    write_to_pin(SRCLK, 0);
    write_to_pin(SER, !cols[i]);
    write_to_pin(SRCLK, 1);
  }

  // Toggle the RCLK pin so that the register outputs are set synchronously
  write_to_pin(RCLK, 1);
}

// display a digit for one cycle
void display_digit(int digit) {
  // Loop through the rows really fast and disply them to give the illusion that all the LEDs are on
  for (int i = 0; i < 7; i++) {
    write_to_row(i, matrices[digit][i]);
    delay(2);
  }
}

void setup() {
  Serial.begin(9600);

  DDRD |= (1 << SER);   // set SER pin as output
  DDRD |= (1 << RCLK);  // set RCLK pin as output
  DDRD |= (1 << SRCLK); // set SRCLK pin as output
}


void loop() {
  // Iterate through the numbers and display them
  for (int i = 0; i < 10; i++) {
    // Display each digit for 50 cycles
    for (int j = 0; j < 50; j++) {
      display_digit(i);
    }
  }
}
