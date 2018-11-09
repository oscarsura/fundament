/*
 * A1: We can control the relative brightness, i.e., the brightness relative to an LED that is
 * on for n = Infinity time, by time-multiplexing based on the brightness contained within the
 * 2d-array. This means that the time delayed in between setting the pmos low and the pmos high
 * can be linearly scaled between 0-15, encapsulating the relative brightness index, scaled
 * by some constant linear default (in the case of our implementation, this is chosen to simply
 * be one, but can be any value that is non-zero).
 */

 /*
  * A2: See below for the implementation of the relative brightness index multi-plexing. A line
  * is specifically marked (commented) in the below implementation that designates the actual change.
  */

const byte ANODE_PINS[8] = {6, 7, 8, 9, 10, 11, 12, 13};
const byte CATHODE_PINS[8] = {A3, A2, A1, A0, 5, 4, 3, 2};

// this is just a const pattern used for testing
const byte my_patt[8][8] = {{0, 0, 0, 0, 0, 0, 0, 0}, 
                            {0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0}};

void setup() {
  //configure all pins as output
  for (byte i = 0; i < 8; i++) {
    pinMode(ANODE_PINS[i], OUTPUT);
    pinMode(CATHODE_PINS[i], OUTPUT);
  }

  //turn on pins off
  for (byte i = 0; i < 8; i++) {
    digitalWrite(ANODE_PINS[i], HIGH);
    digitalWrite(CATHODE_PINS[i], HIGH);
  }

  //set baudrate
  Serial.begin(115200);
  Serial.setTimeout(100);
}

void display(byte pattern[8][8]) {
  for (byte r = 0; r < 8; r++){
    for (byte c = 0; c < 8; c++){
      if (pattern[r][c] != 0) { 
        digitalWrite(CATHODE_PINS[c], LOW); //pull low (on)
      } else { 
        digitalWrite(CATHODE_PINS[c], HIGH); //pull high (off)
      }
    }
    digitalWrite(ANODE_PINS[r], LOW); //turn pmos on
    for (byte c2 = 0; c2 < 8; c2++) { 
      delayMicroseconds(pattern[r][c2]); //A2 brightness index
    }
    digitalWrite(ANODE_PINS[r], HIGH); //turn pmos off
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
