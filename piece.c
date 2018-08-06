#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "piece.h"

static Coordinate PIECES[7][BLOCKS_PER_PIECE] = {
    {
        COORDINATE(0, -1), COORDINATE(0, 0), COORDINATE(0, 1), COORDINATE(-1, 1)
    },
    {
        COORDINATE(0, -1), COORDINATE(0, 0), COORDINATE(0, 1), COORDINATE(1, 1)
    },
    {
        COORDINATE(0, -1), COORDINATE(0, 0), COORDINATE(-1, 0), COORDINATE(-1, 1)
    },
    {
        COORDINATE(0, -1), COORDINATE(0, 0), COORDINATE(1, 0), COORDINATE(1, 1)
    },
    {
        COORDINATE(-1, 0), COORDINATE(0, 0), COORDINATE(0, 1), COORDINATE(1, 0)
    },
    {
        COORDINATE(0, 0), COORDINATE(1, 0), COORDINATE(1, 1), COORDINATE(0, 1)
    },
    {
        COORDINATE(-1, 0), COORDINATE(0, 0), COORDINATE(1, 0), COORDINATE(2, 0),
    }
};

bool piece_init(Piece* piecePtr, enum PieceType type) {
    if (piecePtr == NULL) {
        return false;
    }
    piecePtr->type = type;
    piecePtr->pos.x = 0;
    piecePtr->pos.y = 0;
    int i;
    for (i = 0; i < BLOCKS_PER_PIECE; i++) {
        piecePtr->blocks[i] = PIECES[type-1][i];
    }
    return true;
}

void piece_rotate(Piece* piecePtr, bool counter) {
    if (piecePtr == NULL) {
        return;
    }
    /* Block pieces are special and don't get rotated. */
    if (piecePtr->type == BLOCK_PIECE) {
        return;
    }
    int i;
    Coordinate c;
    for (i = 0; i < BLOCKS_PER_PIECE; i++) {
        c = piecePtr->blocks[i];
        int ox = c.x, oy = c.y;
        if (counter) {
            c.x = oy;
            c.y = -ox;
        } else {
            c.x = -oy;
            c.y = ox;
        }
        piecePtr->blocks[i] = c;
    }
}
