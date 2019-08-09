#include <stdlib.h>
#include <stdbool.h>
#include <curses.h>

#include "board.h"
#include "color.h"

/* Returns if a piece in a particular position or rotation is valid on the
 * given board. */
bool isPieceValid(Board* board, Piece piece);

/* Draws a "chunk" on the board. */
void drawChunk(WINDOW* subWin, int x, int y, int col);

/* Draws a projection so down to where the piece will be */
void drawProjection(WINDOW* subWin, int x, int y, int col);

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
    boardPtr->piece = NULL;
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

void board_setPiece(Board* boardPtr, enum PieceType type) {
    if (boardPtr == NULL) {
        return;
    }
    if (boardPtr->piece == NULL) {
        boardPtr->piece = malloc(sizeof(Piece));
    }
    piece_init(boardPtr->piece, type);
    boardPtr->piece->pos.x = boardPtr->width/2 - 1;

}

bool board_isLineFull(Board* boardPtr, int line) {
    if (boardPtr == NULL || line < 0 || line >= boardPtr->height) {
        return false;
    }
    int i;
    for (i = 0; i < boardPtr->width; i++) {
        if (boardPtr->board[COORD_INDEX(boardPtr, i, line)] == 0) {
            return false;
        }
    }
    return true;
}

bool board_movePiece(Board* boardPtr, int x, int y) {
    if (boardPtr == NULL || boardPtr->piece == NULL) {
        return false;
    }
    Piece newPiece = *(boardPtr->piece);
    Coordinate offset = COORDINATE(x, y);
    newPiece.pos = coordinate_add(newPiece.pos, offset);
    if (!isPieceValid(boardPtr, newPiece)) {
        return false;
    }
    // it's valid, so save it
    *(boardPtr->piece) = newPiece;
    return true;
}

bool board_rotatePiece(Board* boardPtr, bool counter) {
    if (boardPtr == NULL || boardPtr->piece == NULL) {
        return false;
    }
    Piece newPiece = *(boardPtr->piece);
    piece_rotate(&newPiece, counter);
    if (!isPieceValid(boardPtr, newPiece)) {
        return false;
    }
    *(boardPtr->piece) = newPiece;
    return true;
}

void board_cementPiece(Board* boardPtr) {
    if (boardPtr == NULL || boardPtr->piece == NULL) {
        return;
    }
    Piece piece = *(boardPtr->piece);
    int i;
    for (i = 0; i < BLOCKS_PER_PIECE; i++) {
        Coordinate c = coordinate_add(piece.pos, piece.blocks[i]);
        boardPtr->board[COORD_INDEX(boardPtr, c.x, c.y)] = piece.type;
    }
    boardPtr->piece = NULL;
}

void board_draw(Board* boardPtr, WINDOW* subWin, bool showProjection) {
    if (boardPtr == NULL || subWin == NULL) {
        return;
    }
    box(subWin, 0, 0);
    int x;
    for (x = 0; x < boardPtr->width; x++) {
        int y;
        for (y = 0; y < boardPtr->height; y++) {
            int i = COORD_INDEX(boardPtr, x, y);
            unsigned char val = boardPtr->board[i];
            drawChunk(subWin, x, y, val);
        }
    }


    // draw the piece
    if (boardPtr->piece != NULL) {
        int i;
        Piece piece = *(boardPtr->piece);
        // draw the projection to where the piece will be
        if (showProjection) {
            Piece projection = piece;
            while (isPieceValid(boardPtr, projection) && piece.pos.y <= boardPtr->height) {
                projection.pos.y++;
            }
            projection.pos.y--;
            for (i = 0; i < BLOCKS_PER_PIECE; i++) {
                Coordinate c = coordinate_add(projection.pos, projection.blocks[i]);
                if (c.y > 0) {
                    drawProjection(subWin, c.x, c.y, piece.type);
                }
            }
        }

        for (i = 0; i < BLOCKS_PER_PIECE; i++) {
            Coordinate c = coordinate_add(piece.pos, piece.blocks[i]);
            if (c.y > 0) {
                drawChunk(subWin, c.x, c.y, piece.type);
            }
        }
    }
    touchwin(subWin);
    wrefresh(subWin);
}

void board_clearLine(Board* boardPtr, int line) {
    if (boardPtr == NULL || line < 0 || line >= boardPtr->height) {
        return;
    }
    int i;
    for (i = 0; i < boardPtr->width; i++) {
        boardPtr->board[COORD_INDEX(boardPtr, i, line)] = 0;
    }
}

void board_shiftLine(Board* boardPtr, int line) {
    if (boardPtr == NULL || line < 0 || line >= boardPtr->height) {
        return;
    }
    int x;
    for (x = 0; x < boardPtr->width; x++) {
        int y;
        for (y = line; y > 0; y--) {
            boardPtr->board[COORD_INDEX(boardPtr, x, y)] =
                boardPtr->board[COORD_INDEX(boardPtr, x, y-1)];
        }
        boardPtr->board[COORD_INDEX(boardPtr, x, 0)] = 0;
    }
}

void board_clear(Board* boardPtr) {
    if (boardPtr == NULL) {
        return;
    }
    int i;
    for (i = 0; i < boardPtr->width*boardPtr->height; i++) {
        boardPtr->board[i] = 0;
    }
}

bool isPieceValid(Board* boardPtr, Piece piece) {
    int i;
    for (i = 0; i < BLOCKS_PER_PIECE; i++) {
        Coordinate c = coordinate_add(piece.pos, piece.blocks[i]);
        if (c.x >= boardPtr->width || c.x < 0 ||
                c.y >= boardPtr->height) {
            return false;
        }
        // make sure that it's empty
        unsigned char val = boardPtr->board[COORD_INDEX(boardPtr, c.x, c.y)];
        if (val > 0) {
            return false;
        }
    }
    return true;
}

void drawChunk(WINDOW* subWin, int x, int y, int col) {
    attr_t attrs = COLOR_PAIR(col);
    wattron(subWin, attrs);
    mvwprintw(subWin, y+1, (x*2)+1, "  ");
    wattroff(subWin, attrs);
}

void drawProjection(WINDOW* subWin, int x, int y, int col) {
    attr_t attrs = COLOR_PAIR(col + 8);
    wattron(subWin, attrs);
    mvwprintw(subWin, y+1, (x*2)+1, "::");
    wattroff(subWin, attrs);
}
