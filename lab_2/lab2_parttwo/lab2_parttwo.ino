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

#define SER 4
#define RCLK 5
#define SRCLK 6

// Write to a row given the columns that should be turned on in that row
// for example, if row = 0 and cols = {0, 1, 1 1, 0}, then the top row is set to off, on, on, on, off
void write_to_row(int row, int cols[]) {
  digitalWrite(RCLK, 0);

  for (int i = 4; i >= 0; i--) {
    digitalWrite(SRCLK, 0);
    delay(1);
    digitalWrite(SER, cols[i]);
    delay(1);
    digitalWrite(SRCLK, 1);
  }

  for (int i = 6; i >= 0; i--) {
    digitalWrite(SRCLK, 0);
    delay(1);
    digitalWrite(SER, i != row);
    delay(1);
    digitalWrite(SRCLK, 1);
  }

  digitalWrite(RCLK, 1);
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

  pinMode(SER, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(SRCLK, OUTPUT);
}


void loop() {
  for (int i = 0; i < 10; i++) {
    Serial.println(i);
    for (int j = 0; j < 50; j++) {
      display_digit(i);
    }
  }
}
