#include <stdlib.h>
#include <stdbool.h>

#include "board.h"

Board* board_init(int width, int height) {
   Board* boardPtr = malloc(sizeof(Board));
   if (boardPtr == NULL) {
       return NULL;
   }
   boardPtr->width = width;
   boardPtr->height = height;
   boardPtr->board = calloc((unsigned long)(width*height), sizeof(char));
   if (boardPtr->board == NULL) {
       free(boardPtr);
       return NULL;
   }
   return boardPtr;
}

void board_free(Board* boardPtr) {
    if (boardPtr != NULL) {
        if (boardPtr->board != NULL) {
            free(boardPtr->board);
        }
        free(boardPtr);
    }
}

bool board_setPiece(Board* boardPtr, Piece piece) {
    if (boardPtr == NULL) {
        return false;
    }
    Piece oldPiece = boardPtr->piece; // for safekeeping later
    boardPtr->piece = piece;
    int i;
    for (i = 0; i < BLOCKS_PER_PIECE; i++) {
        Coordinate c = coordinate_add(piece.pos, piece.blocks[i]);
        if (boardPtr->board[COORD_INDEX(boardPtr, c.x, c.y)] > 0) {
            boardPtr->piece = oldPiece;
            return false;
        }
    }
    // it's valid
    return true;
}
