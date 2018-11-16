const byte ANODE_PINS[8] = {6, 7, 8, 9, 10, 11, 12, 13}; //led anode pin
const byte CATHODE_PINS[8] = {A3, A2, A1, A0, 5, 4, 3, 2}; //led cathod pin
const byte POT_PINS[2] = {A5, A4}; //potentiometer pins

//define a point_t type
struct point_t {
    byte r, c;
    point_t () {
       r = 0;
       c = 0;
    }
};

//define a grid_t type
struct grid_t {
    byte grid[8][8];

    //default constructor
    grid_t(byte g[8][8]) {
        for (byte r = 0; r < 8; r++) {
            for (byte c = 0; c < 8; c++) {
                grid[r][c] = g[r][c];
            }
        }
    }

    //display a grid, by time multiplexing through its elements
    void disp() {
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
                delayMicroseconds(grid[r][c2]*1000); //A2 brightness index
            }
            digitalWrite(ANODE_PINS[r], HIGH); //turn pmos off
        }
    }

    //set an individual led's value
    void set_led(byte row, byte col, byte val) {
        grid[row][col] = val;
    }

    //clear an entire row
    void clear_row(byte row) {
        for (byte c = 0; c < 8; c++) {
            grid[row][c] = 0;
        }
    }

    //set an entire row, otherwise equivalent to clear for 0
    void set_row(byte row, byte val) {
        for (byte c = 0; c < 8; c++) {
            grid[row][c] = val;
        }
    }

    //clear the entire grid
    void clear_grid() {
        for (int i = 0; i < 8; i++) {
            clear_row(i);
        }
    }
};

//define a paddle_t type
struct paddle_t {
    byte row;
    byte midpt;

    //default constructor
    paddle_t() {
        row = 7; //default row
        midpt = 3; //default col
    }

    //init constructor
    paddle_t(byte r, byte m) {
        row = r;
        midpt = m;
    }

    //set the midpoint of the paddle
    void set_midpt(byte i) {
        midpt = i;
    }

    //update the grid by reference
    void updateg(grid_t& grid) {
        if (midpt < 0) midpt = 0;
        if (midpt > 7) midpt = 7;
        grid.clear_row(row);
        for (int c = midpt - 1; c <= midpt + 1; c++) {
            //check within bounds
            if ((row >= 0 && row <= 7) && (c >= 0 && c <= 7)) {
                grid.set_led(row, c, 1);
            }
        }
    }
};

//define a ball_t type
struct ball_t {
    int r, c;
    int vel_r, vel_c;

    //default constructor
    ball_t() {
      r = random(2,5);
      c = random(2,5);
      vel_r = random(0,2) ? - 1 : 1;
      vel_c = random(0,2) ? - 1 : 1;
    }

    //set location of the ball
    void set_loc(int row, int col) {
        r = row;
        c = col;
    }

    //update the grid by reference
    void updateg(grid_t& grid, paddle_t& p) {
        r = r + vel_r;
        c = c + vel_c;

        //interaction with paddle
        if (r == 6) {
            int midpt = p.midpt;
            if (abs(midpt - c) <= 1) {
                vel_r = -vel_r;
                if (vel_c == 0) {
                    vel_c = random(0,2) ? - 1 : 1;
                }
            }
        }

        //interaction with sides of wall
        if (c == 0 || c == 7) {
            vel_c = -vel_c;
            if (vel_r == 0) {
                vel_r = random(0,2) ? - 1 : 1;
            }
        }

        //interaction with bottom wall
        if (r == 1) {
           vel_r = -vel_r;
           if (vel_c == 0) {
                vel_c = random(0,2) ? - 1 : 1;
            }
        }

        //set the ball location
        if ((r >= 0 && r <= 7) && (c >= 0 && c <= 7)) {
            grid.set_led(r, c, 1);
        } else {
            r = random(2,5);
            c = random(2,5);
            vel_r = random(0,2) ? - 1 : 1;
            vel_c = random(0,2) ? - 1 : 1;
        }
    }

    //get r location of ball
    int get_r() {
        return r;
    }

    //get c location of ball
    int get_c() {
        return c;
    }
};

//init the game state
void setup() {
    //configure pins as output
    for (byte i = 0; i < 8; i++) {
        pinMode(ANODE_PINS[i], OUTPUT);
        pinMode(CATHODE_PINS[i], OUTPUT);
    }

    //configure pins as input
    pinMode(POT_PINS[0], INPUT);
    pinMode(POT_PINS[1], INPUT);

    //turn all pins off
    for (byte i = 0; i < 8; i++) {
        digitalWrite(ANODE_PINS[i], HIGH);
        digitalWrite(CATHODE_PINS[i], HIGH);
    }

    //set baudrate
    Serial.begin(115200);
    Serial.setTimeout(100);

    //init variables
    randomSeed(analogRead(POT_PINS[1]));
}

//map input to output desired values
int map_val(int val, int r_min, int r_max, int t_min, int t_max) {
    return val / (r_max / t_max);
}

//update the location of the paddle based on the pot value
void update_paddle(grid_t& grid, paddle_t& p) {
    int val = analogRead(POT_PINS[0]);
    int midpt = map_val(val, 0, 1023, 0, 7);
    p.set_midpt(midpt);
    p.updateg(grid);
}

//draw the bottom wall
void draw_wall(grid_t& grid) {
    grid.set_row(0, 1);
}

//draw the location of the ball, for use when ball is frozen
void draw_ball(grid_t& grid, int r, int c) {
    grid.set_led(r, c, 1);
}

//update the location of ball based on paddle location
void update_ball(grid_t& grid, ball_t& b, paddle_t& p) {
    b.updateg(grid, p);
}

//game loop and game state
const unsigned long update_base = 200;
void loop() {
    //init state
    byte g[8][8] = {{1,1,1,1,1,1,1,1},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,1,1,1,0,0,0}};
    grid_t grid(g);
    paddle_t paddle;
    ball_t ball;
    unsigned long now = millis();

    //game loop
    while(true) {
        //set the time for ball speed
        unsigned long update_t = update_base + analogRead(POT_PINS[1]);

        //reset and redraw
        grid.clear_grid();
        draw_wall(grid);
        update_paddle(grid, paddle);

        //sufficient time to update ball
        if (millis() - now > update_t) {
            update_ball(grid, ball, paddle);
            now = millis();
        }
        draw_ball(grid, ball.r, ball.c);

        //display the grid
        grid.disp();
    }
}
