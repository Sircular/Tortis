/* Defines a piece struct that contains information about the location of
 * blocks (including rotation) and the location on the board. Validation is
 * generally done by the board. */

#ifndef TETRIS_PIECE
#define TETRIS_PIECE

#include "coordinate.h"

#define BLOCKS_PER_PIECE 4

/* Piece types enum. */
#define PIECE_TYPES_COUNT 6
enum PieceType {
    L_L_PIECE,
    R_L_PIECE,
    L_ZIGZAG,
    R_ZIGZAG,
    T_PIECE,
    LONG_PIECE,
    NONE_PIECE
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
