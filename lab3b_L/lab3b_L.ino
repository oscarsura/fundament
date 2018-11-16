const byte ANODE_PINS[8] = {6, 7, 8, 9, 10, 11, 12, 13};
const byte CATHODE_PINS[8] = {A3, A2, A1, A0, 5, 4, 3, 2};
const byte POT_PINS[2] = {A5, A4};

struct paddle_t {
    byte row;
    byte midpt;

    paddle_t() {
        row = 7; //default row
        midpt = 3; //default col
    }

    paddle_t(byte r, byte m) {
        row = r;
        midpt = m;
    }

    set_midpt(byte i) {
        midpt = i;
    }

    draw() {}
 };

char grid[8][8] = {{1,1,1,1,1,1,1,1},
                   {0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0},
                   {0,0,1,1,1,0,0,0}};

paddle_t p;

void setup() {
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
  
    //init variables
}

void display() {
 for (byte r = 0; r < 8; r++) {
   for (byte c = 0; c < 8; c++) {
     if (grid[r][c] != 0) {
       digitalWrite(CATHODE_PINS[c], LOW); //pull low (on)
     } else {
       digitalWrite(CATHODE_PINS[c], HIGH); //pull high (off)
     }
   }
   digitalWrite(ANODE_PINS[r], LOW); //turn pmos on
   for (byte c2 = 0; c2 < 8; c2++) {
     delayMicroseconds(grid[r][c2]); //A2 brightness index
   }
   digitalWrite(ANODE_PINS[r], HIGH); //turn pmos off
 }
}

/*
 * Method: map_val
 * Usage: int newVal = map_val(val, r_min, r_max, t_min, t_max);
 * -------------------------------------------------------------
 * This method will scale a value from the measurement range
 * [r_min, r_max] to the desired range [t_min, t_max].
 * Mathematical derivation sourced from stats.stackexchange.com
 * questions/281162/scale-a-number-between-a-range
 */
int map_val(int val, int r_min, int r_max, int t_min, int t_max) {
    int quotient = (val - r_min)/(r_max - r_min);
    int diff = (t_max - t_min);
    return quotient*diff + t_min;
}

/*
 * Method: updatePaddle
 * Usage: updatePaddle(...);
 * -----------------------
 * This method will update the location of the paddle based on the digital
 * values read from the Arduino's input pin, connected to the potentiometers.
 */
void updatePaddle() {
    int val = digitalRead(POT_PINS[0]);
    map(val, 0, 1023, 0, 7);

}

/*
 * Method: drawScreen
 * Usage: drawScreen(...);
 * -----------------------
 * drawScreen returns void, and is a wrapper around the display function,
 * but may add additional parameter or state checking around the object itself.
 */
void drawScreen() {
    display();
}

void loop() {
    while(true) {
        updatePaddle();
        //updateBall();
        drawScreen();
    }
}
