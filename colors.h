/*
 * Sets up colors for rendering the board. Uses the piece enums as indices for
 * the color pairs.
 */


#ifndef TETRIS_COLOR
#define TETRIS_COLOR

#include <ncurses.h>

enum Color {
    L_L_COLOR = COLOR_YELLOW,
    R_L_COLOR = COLOR_BLUE,
    L_ZIGZAG_COLOR = COLOR_GREEN,
    R_ZIGZAG_COLOR = COLOR_RED,
    T_COLOR = COLOR_MAGENTA,
    BLOCK_COLOR = COLOR_WHITE,
    LONG_COLOR = COLOR_CYAN
};

/* Initializes color pairs. */
void colors_init(void);

#endif
