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
//  
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

// Check if the button was pressed and inturrupt sevice routine was triggered
void button_interrupt() {
  /*
  curr_digit = (curr_digit +1) % 10;
  detachInterrupt(digitalPinToInterrupt(2));
  delay(debounce_delay);
  attachInterrupt(digitalPinToInterrupt(2), button_interrupt, FALLING);   
  */
  long curr_time = millis();

  // Check to ensure no two buttons occur within 200 milliseconds, if not, 
  // update count
  long time_difference = curr_time - last_time;
  if (time_difference > debounce_delay) {
      // update current digit, ensure that it wraps around 0-9
      curr_digit = (curr_digit + 1) % 10; // use modulo for wrapping
      last_time = curr_time;
  }
}


void display_digit(int digit) {
  // display the digit
  for (int i = 0; i < 7; i++) {
    write_to_row(i, matrices[digit][i]);
    delay(1);
  }
}

void setup() {
  Serial.begin(9600);

  DDRD |= (1 << SER);   // set SER pin as output
  DDRD |= (1 << RCLK);  // set RCLK pin as output
  DDRD |= (1 << SRCLK); // set SRCLK pin as output
  
  // Set pin2 as input pullup
  pinMode(2, INPUT_PULLUP);

  // Attach inturrupt to PD2
  attachInterrupt(digitalPinToInterrupt(2), button_interrupt, FALLING); 
}



void loop() {
    display_digit(curr_digit);
}
