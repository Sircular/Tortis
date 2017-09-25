/* Defines a piece struct that contains information about the location of
 * blocks (including rotation) and the location on the board. Validation is
 * generally done by the board. */

#ifndef TETRIS_PIECE
#define TETRIS_PIECE

#include "coordinate.h"

#define BLOCKS_PER_PIECE 4

/* Piece types enum. */
#define PIECE_TYPES_COUNT 7
enum PieceType {
    L_L_PIECE = 1,
    R_L_PIECE = 2,
    L_ZIGZAG_PIECE = 3,
    R_ZIGZAG_PIECE = 4,
    T_PIECE = 5,
    BLOCK_PIECE = 6,
    LONG_PIECE = 7
};

/* Piece struct. */
typedef struct Piece {
    enum PieceType type;
    Coordinate pos;
    Coordinate blocks[BLOCKS_PER_PIECE];
} Piece;

/* Initializes a piece (by filling its blocks.) Assumes that the memory is
 * already allocated.*/
bool piece_init(Piece* piecePtr, enum PieceType type);

/* Rotate a piece to clockwise and counterclockwise. Works by working on
 * individual x and y coordinates. */
void piece_rotate(Piece* piecePtr, bool counter);

#endif
