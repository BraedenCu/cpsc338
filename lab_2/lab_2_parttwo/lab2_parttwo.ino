// digit 0
int matrix_0[5][7] = {
  {0, 1, 1, 1, 0},
  {0, 1, 0, 1, 0},
  {0, 1, 0, 1, 0},
  {0, 1, 0, 1, 0},
  {0, 1, 1, 1, 0}
};

// digit 1
int matrix_1[5][7] = {
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0}
};

// digit 2
int matrix_2[5][7] = {
  {0, 1, 1, 1, 0},
  {0, 0, 0, 1, 0},
  {0, 1, 1, 1, 0},
  {0, 1, 0, 0, 0},
  {0, 1, 1, 1, 0}
};

// digit 3
int matrix_3[5][7] = {
  {0, 1, 1, 1, 0},
  {0, 0, 0, 1, 0},
  {0, 1, 1, 1, 0},
  {0, 0, 0, 1, 0},
  {0, 1, 1, 1, 0}
};

// digit 4
int matrix_4[5][7] = {
  {0, 1, 0, 1, 0},
  {0, 1, 0, 1, 0},
  {0, 1, 1, 1, 0},
  {0, 0, 0, 1, 0},
  {0, 0, 0, 1, 0}
};

// digit 5
int matrix_5[5][7] = {
  {0, 1, 1, 1, 0},
  {0, 1, 0, 0, 0},
  {0, 1, 1, 1, 0},
  {0, 0, 0, 1, 0},
  {0, 1, 1, 1, 0}
};

// digit 6
int matrix_6[5][7] = {
  {0, 1, 1, 1, 0},
  {0, 1, 0, 0, 0},
  {0, 1, 1, 1, 0},
  {0, 1, 0, 1, 0},
  {0, 1, 1, 1, 0}
};

// digit 7
int matrix_7[5][7] = {
  {0, 1, 1, 1, 0},
  {0, 0, 0, 1, 0},
  {0, 0, 0, 1, 0},
  {0, 0, 0, 1, 0},
  {0, 0, 0, 1, 0}
};

// digit 8
int matrix_8[5][7] = {
  {0, 1, 1, 1, 0},
  {0, 1, 0, 1, 0},
  {0, 1, 1, 1, 0},
  {0, 1, 0, 1, 0},
  {0, 1, 1, 1, 0}
};

// digit 9
int matrix_9[5][7] = {
  {0, 1, 1, 1, 0},
  {0, 1, 0, 1, 0},
  {0, 1, 1, 1, 0},
  {0, 0, 0, 1, 0},
  {0, 1, 1, 1, 0}
};

char* names[10] = {
  "matrix_0", 
  "matrix_1",
  "matrix_2",
  "matrix_3",
  "matrix_4",
  "matrix_5",
  "matrix_6",
  "matrix_7",
  "matrix_8",
  "matrix_9"
};

void display_digit(int digit) {
  // display the digit
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 7; j++) {
      // display the digit
      // Serial.print(matrix[digit][i][j]);
      Serial.print(names[digit]);
    }
    Serial.println();
  }
}

void setup() {
  Serial.begin(9600);
  
}


void loop() {
  for (int i = 0; i < 10; i++) {
    display_digit(i);
    delay(1000);
  }
}
