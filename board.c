#include <stdlib.h>
#include <stdbool.h>
#include <curses.h>

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
    if (piece.type != NONE_PIECE) {
        int i;
        for (i = 0; i < BLOCKS_PER_PIECE; i++) {
            Coordinate c = coordinate_add(piece.pos, piece.blocks[i]);
            if (boardPtr->board[COORD_INDEX(boardPtr, c.x, c.y)] > 0) {
                boardPtr->piece = oldPiece;
                return false;
            }
        }
    }
    // it's valid
    return true;
}

bool board_draw(Board* boardPtr, WINDOW* subWin) {
    if (boardPtr == NULL || subWin == NULL) {
        return false;
    }
    box(subWin, 0, 0);
    int x;
    for (x = 0; x < boardPtr->width; x++) {
        int y;
        for (y = 0; y < boardPtr->height; y++) {
            int i = COORD_INDEX(boardPtr, x, y);
            // TODO: drawing with colors
            if (boardPtr->board[i] > 0) {
                mvwprintw(subWin, y+1, (x*2)+1, "XX");
            } else {
                mvwprintw(subWin, y+1, (x*2)+1, "  ");
            }
        }
    }
    touchwin(subWin);
    return true;
}
