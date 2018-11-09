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
}

void loop() {
  for (byte r = 0; r < 8; r++) {
    digitalWrite(ANODE_PINS[r], LOW); //activate pmos
    for (byte c = 0; c < 8; c++) {
      digitalWrite(CATHODE_PINS[c], LOW); //pull low
      delay(100);
      digitalWrite(CATHODE_PINS[c], HIGH); //pull high
    }
    digitalWrite(ANODE_PINS[r], HIGH); //deactivate pmos
  }
}
