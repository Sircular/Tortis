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
#include "grabbag.h"
#include "color.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#define GRABBAG_REPETITIONS 4

static Board* board;

static WINDOW* gameWin;
static WINDOW* boardWin;

static GrabBag* grabBag;

/* Initializes the game window and board window. */
void initWindows(void);

/* Redraws all of the windows and refreshes the screen. */
void redrawGame(void);

/* The main game loop. */
void gameLoop(void);

int main() {
    // set up the screen
    initscr();
    raw();
    noecho();
    curs_set(0);
    refresh();
    colors_init();
    board = board_init(BOARD_WIDTH, BOARD_HEIGHT);
    // initialize the windows
    initWindows(); 
    // initialize the grab bag for piece selection
    grabBag = grabbag_init(GRABBAG_REPETITIONS);
    board_setPiece(board, T_PIECE);

    redrawGame();
    gameLoop();

    board_free(board);
    grabbag_free(grabBag);
    endwin();
    return 0;
}

void initWindows() {
    int boardWinWidth = (BOARD_WIDTH*2) + 2;
    int winHeight = BOARD_HEIGHT+2;
    int totalWidth = boardWinWidth+BOARD_WIDTH;

    int tWidth, tHeight;
    getmaxyx(stdscr, tHeight, tWidth);

    board = board_init(BOARD_WIDTH, BOARD_HEIGHT);
    gameWin = newwin(winHeight, totalWidth, (tHeight-winHeight)/2,
            (tWidth-totalWidth)/2);
    boardWin = derwin(gameWin, winHeight, boardWinWidth, 0, 0);
}

void redrawGame() {
    board_draw(board, boardWin);
    wrefresh(gameWin);
    refresh();
}

void gameLoop() {
    bool running = true;
    while (running) {
        board_rotatePiece(board, false);
        redrawGame();
        if (getch() == 10) {
            running = false;
        }
        if (!board_movePiece(board, 0, 1)) {
            running = false;
        }
    }
}
