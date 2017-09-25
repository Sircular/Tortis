/*
 * Tetris Clone
 * Walter Mays
 *
 * Entry Point
 */

#include <curses.h>
#include <stdbool.h>
#include <sys/select.h>
#include <sys/time.h>

#include "board.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

static Board* board;

static WINDOW* gameWin;
static WINDOW* boardWin;

int main() {
    initscr();
    raw();
    noecho();
    //nodelay(stdscr, true);
    refresh();

    {
        int boardWinWidth = (BOARD_WIDTH*2) + 2;
        int winHeight = BOARD_HEIGHT+2;
        int totalWidth = boardWinWidth+BOARD_WIDTH;
        board = board_init(BOARD_WIDTH, BOARD_HEIGHT);
        gameWin = newwin(winHeight, totalWidth, 0, 0);
        boardWin = derwin(gameWin, winHeight, boardWinWidth, 0, 0);
    }

    int i;
    for (i = 0; i < BOARD_WIDTH; i++) {
        board->board[COORD_INDEX(board, i, BOARD_HEIGHT-1)] = 1;
    }
    board_draw(board, boardWin);
    wrefresh(gameWin);
    refresh();
    getch();

    board_free(board);
    endwin();
    return 0;
}
