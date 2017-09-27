/*
 * Defines all the necessary functions and structs to keep track of the various
 * blocks onscreen.
 */

#ifndef TETRIS_BOARD
#define TETRIS_BOARD

#include <curses.h>
#include <stdbool.h>

#include "piece.h"

/* Board struct. Contains width, height, board contents, and the current piece
 * that is being manipulated by the player. */
typedef struct Board {
    int width;
    int height;
    unsigned char* board;
    Piece* piece;
} Board;

/* Gets the index from the x and y coordinates based on width. */
#define COORD_INDEX(board, x, y) (x) + (board->width * (y))

/* Initializes a board of the specified dimensions. */
Board* board_init(int width, int height);

/* Frees a board. Returns true on success. */
void board_free(Board* boardPtr);

/* Sets the current falling piece. */
void board_setPiece(Board* boardPtr, enum PieceType type);

/* Determines if line <line> is completely full. */
bool board_isLineFull(Board* boardPtr, int line);

/* Attempts to move the piece by the specified coordinates. Returns true if it
 * is a valid move. */
bool board_movePiece(Board* boardPtr, int x, int y);

/* Attempts to rotation the piece by the specified coordinates. Returns true if
 * it is a valid rotation. */
bool board_rotatePiece(Board* boardPtr, bool counter);

/* "Cements" the current piece onto the board. Called when a block falls to its
 * lowest point. */
void board_cementPiece(Board* boardPtr);

/* Draws the board on the specified window at (0,0). Returns true on success.
 * */
void board_draw(Board* boardPtr, WINDOW* subWin);
#endif
