#include <curses.h>

#include "piece.h"
#include "color.h"

void colors_init() {
    use_default_colors();
    start_color();
    init_pair(L_L_PIECE, COLOR_BLACK, L_L_COLOR);
    init_pair(R_L_PIECE, COLOR_BLACK, R_L_COLOR);
    init_pair(L_ZIGZAG_PIECE, COLOR_BLACK, L_ZIGZAG_COLOR);
    init_pair(R_ZIGZAG_PIECE, COLOR_BLACK, R_ZIGZAG_COLOR);
    init_pair(T_PIECE, COLOR_BLACK, T_COLOR);
    init_pair(BLOCK_PIECE, COLOR_BLACK, BLOCK_COLOR);
    init_pair(LONG_PIECE, COLOR_BLACK, LONG_COLOR);
}
