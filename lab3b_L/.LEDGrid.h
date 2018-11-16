#ifndef LEDGRID_H
#define LEDGRID_H

class LEDGrid {
    public:
        LEDGrid();
        void clear_screen();
        void set_grid(char grid[8][8]);
        void set_index(char r, char c, char val);
        LEDGrid get_grid();
        char get_index();

    private:
        char grid[8][8] = {{0,0,0,0,0,0,0,0},
                           {0,0,0,0,0,0,0,0},
                           {0,0,0,0,0,0,0,0},
                           {0,0,0,0,0,0,0,0},
                           {0,0,0,0,0,0,0,0},
                           {0,0,0,0,0,0,0,0},
                           {0,0,0,0,0,0,0,0},
                           {0,0,0,0,0,0,0,0}};
};
#endif
