/*
 * Defines all the necessary functions and structs to keep track of the various
 * blocks onscreen.
 */

#ifndef TETRIS_BOARD
#define TETRIS_BOARD

#include <curses.h>

#include "piece.h"

/* Board struct. Contains width, height, board contents, and the current piece
 * that is being manipulated by the player. */
typedef struct Board {
    int width;
    int height;
    char* board;
    Piece piece;
} Board;

/* Gets the index from the x and y coordinates based on width. */
#define COORD_INDEX(board, x, y) x + (board->width * y)

/* Initializes a board of the specified dimensions. Returns true on success. */
Board* board_init(int width, int height);

/* Frees a board. Returns true on success. */
void board_free(Board* boardPtr);

/* Attempts to set a piece as the piece on the board. Returns true if it is
 * valid and false if invalid. */
bool board_setPiece(Board* boardPtr, Piece piece);

/* Draws the board on the specified window at (0,0). Returns true on success.
 * */
bool board_draw(Board* boardPtr, WINDOW* winPtr);
#endif
