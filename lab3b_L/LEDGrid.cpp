#include "LEDGrid.h"

LEDGrid::LEDGrid() {
    this.grid = {{0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0}};
}

LedGrid::LEDGrid(byte[8][8] grid) {
    this.grid = grid;
}

void LEDGrid::draw() {
    return;
}
void LEDGrid::clear() {
    return;
}
void LEDGrid::set_grid(byte[][] grid) {
    return;
}

void LEDGrid::set_index(byte r, byte c, byte val) {
    return;
}
