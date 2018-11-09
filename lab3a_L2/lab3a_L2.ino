const byte ANODE_PINS[8] = {6, 7, 8, 9, 10, 11, 12, 13};
const byte CATHODE_PINS[8] = {A3, A2, A1, A0, 5, 4, 3, 2};

void setup() {
  //configure all pins as output
  for (byte i = 0; i < 8; i++) {
    pinMode(ANODE_PINS[i], OUTPUT);
    pinMode(CATHODE_PINS[i], OUTPUT);
  }

  //turn all pins off
  for (byte i = 0; i < 8; i++) {
    digitalWrite(ANODE_PINS[i], HIGH);
    digitalWrite(CATHODE_PINS[i], HIGH);
  }

  //set baudrate
  Serial.begin(115200);
  Serial.setTimeout(100);
}

/*
 * Method: display
 * Usage: display(myPattern);
 * --------------------------
 * This function allows a layer of abstraction
 * to be formed over turning on each LED in the
 * dot matrix time-multiplexed display.
 */
void display(byte pattern[8][8]) {
  for (byte r = 0; r < 8; r++){
    for (byte c = 0; c < 8; c++){
      if (pattern[r][c] != 0) { 
        digitalWrite(CATHODE_PINS[c], LOW); //turn on
      } else { 
        digitalWrite(CATHODE_PINS[c], HIGH); //turn off
      }
    }
    digitalWrite(ANODE_PINS[r], LOW); //turn on
    delayMicroseconds(500); //delay
    digitalWrite(ANODE_PINS[r], HIGH); //turn off
  }
}

void loop() {
  static byte ledOn[8][8];

  byte x = 0;
  byte y = 0;
  static char message[60];

  if (Serial.available()) {
    x = Serial.parseInt();
    y = Serial.parseInt();

    if (Serial.read() != '\n') {
      Serial.println("invalid input - check that line ending is set to \"Newline\"; input must be two numbers");
      return;
    }
    if (x < 0 || x > 7 || y < 0 || y > 7) {
      sprintf(message, "x = %d, y = %d -- indices out of bounds", x, y);
      Serial.println(message);
      return;
    }

    sprintf(message, "x = %d, y = %d", x, y);
    Serial.println(message);

    ledOn[x][y] = !ledOn[x][y];
  }

  display(ledOn);
}
