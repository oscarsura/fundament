#include "LEDGrid.h"

LEDGrid::LEDGrid() {}

void LEDGrid::clear_screen() {
    for (char i = 0; i < 8; i++) {
        for (char j = 0; j < 8; j++) {
            set_index(i,j,0);
        }
    }
}
void LEDGrid::set_grid(char g[...][8]) {
    grid = g;
}

void LEDGrid::set_index(char r, char c, char val) {
    grid[r][c] = val;
}
