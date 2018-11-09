const int GATE1 = 12; //drive the first gate of the transistor
const int GATE2 = 11; //drive the second gate of the transistor
const int TOGGLE = 2; //read the value of the toggle switch
const int LIMIT = 10; //read the value of the limit switch

void setup() {
  Serial.begin(115200); //set baudrate
  pinMode(GATE1, OUTPUT); //drive gate1 pin
  pinMode(GATE2, OUTPUT); //drive gate2 pin
  pinMode(TOGGLE, INPUT_PULLUP); //read toggle pin
  pinMode(LIMIT, INPUT_PULLUP); //read limit pin
  Serial.println("Setup completed");
}

void forward() {
  digitalWrite(GATE1, HIGH);
  digitalWrite(GATE2, LOW);
}

void reverse() {
  digitalWrite(GATE1, LOW);
  digitalWrite(GATE2, HIGH);
}

void stopm() {
  digitalWrite(GATE1, LOW);
  digitalWrite(GATE2, LOW);
}

static int on_count = 0; //to store iterations toggle on
static int off_count = 0; //to store iterations toggle off
void loop() {
  boolean toggle_on = digitalRead(TOGGLE);
  boolean limit_closed = digitalRead(LIMIT);

  //react to toggle flip on/off
  if (toggle_on) { //need to move forward
    if (on_count > 5000) {
      //the machine will "break itself" and never move again
      //if the toggle switch is flipped too many times
      return; 
    }
    forward();
    on_count++;
  } else {
    off_count++;
  }

  //retract
  if (!toggle_on && !limit_closed) {
    reverse();
  }

  //stop moving
  if (!toggle_on && limit_closed) {
    stopm();
  }

  //tease
  if (off_count > 10000) {
    for (int i = 0; i < random(1000, 10000); i++) {
      forward();
    }
    off_count = 0;
  }
}
