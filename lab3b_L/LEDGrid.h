#ifndef LEDGRID_H
#define LEDGRID_H

class LEDGrid {
    public:
        LEDGrid();
        LEDGrid(byte[][]);
        void draw();
        void clear();
        void set_grid(byte[][] grid);
        void set_index(byte r, byte c, byte val);

    private:
        byte[8][8] grid;
};

#endif
