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
#include "menu.h"
#include "keys.h"

#define MICROS_TO_SECONDS 1000000

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#define GRABBAG_REPETITIONS 4
#define TIMEOUT 1000000 // in microseconds

#define CHOICE_STRS {"Play", "Exit"}
#define CHOICE_NUM 2
#define CHOICE_TITLE "Main Menu"
#define CHOICE_PLAY 0
#define CHOICE_EXIT 1

static Board* board;

static WINDOW* gameWin;
static WINDOW* boardWin;

static GrabBag* grabBag;

/* Performs curses and game struct initialization. */
void setup(Board** boardRef, GrabBag** bagRef);

/* Initializes the game window and board window. */
void initWindows(void);

/* Redraws all of the windows and refreshes the screen. */
void redrawGame(void);

/* The main game loop. */
void gameLoop(void);

/* Gets input and simulates the block until it is locked into place. */
void dropBlock(long timeout);

/* Shows the main menu and returns the choice made. */
int showMainMenu(void);

int main() {
    setup(&board, &grabBag);
    board_setPiece(board, T_PIECE);

    redrawGame();
    bool running = true;
    int choice;
    while (running) {
        choice = showMainMenu();
        endwin();
        switch (choice) {
            case CHOICE_PLAY:
                gameLoop();
                break;
            case CHOICE_EXIT:
                running = false;
                break;
            default:
                // We don't know what else to do
                running = false;
        }
    }

    board_free(board);
    grabbag_free(grabBag);
    delwin(boardWin);
    delwin(gameWin);
    endwin();
    return 0;
}

void setup(Board** boardRef, GrabBag** bagRef) {
    initscr();
    raw();
    noecho();
    curs_set(0);
    refresh();
    colors_init();
    *boardRef = board_init(BOARD_WIDTH, BOARD_HEIGHT);
    // initialize the windows
    initWindows(); 
    // initialize the grab bag for piece selection
    *bagRef = grabbag_init(GRABBAG_REPETITIONS);
}

void initWindows() {
    int boardWinWidth = (BOARD_WIDTH*2) + 2;
    int winHeight = BOARD_HEIGHT+2;
    int totalWidth = boardWinWidth+BOARD_WIDTH;

    int tWidth, tHeight;
    getmaxyx(stdscr, tHeight, tWidth);

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

    dropBlock(TIMEOUT);
}

void dropBlock(long timeout) {
    bool falling = true;
    struct timeval time, tmp_time;
    // setup inputs for select
    fd_set in, tmp_in;

    FD_ZERO(&in);
    FD_SET(0, &in); //adds stdin

    time.tv_sec = timeout/MICROS_TO_SECONDS;
    time.tv_usec = timeout%MICROS_TO_SECONDS;
    tmp_time = time;

    while (falling) {
        redrawGame();
        tmp_in = in; // so we don't overwrite the stream
        int sel_value = select(FD_SETSIZE, &tmp_in, NULL, NULL, &tmp_time);
        if (sel_value != 0) { // we didn't timeout
            int c = getch();
            switch(c) {
                case KEY_UP:
                    board_rotatePiece(board, false);
                    break;
                case KEY_LEFT:
                    board_movePiece(board, -1, 0);
                    break;
                case KEY_RIGHT:
                    board_movePiece(board, 1, 0);
                    break;
                case KEY_DOWN:
                    if (board_movePiece(board, 0, 1)) {
                        // reset the timer
                        tmp_time = time;
                    } else {
                        falling = false;
                    }
                    break;
            }
        } else { // we timed out
            if (board_movePiece(board, 0, 1)) {
                // reset the timer
                tmp_time = time;
            } else {
                falling = false;
            }
        }
    }
}

int showMainMenu() {
    char* choices[] = CHOICE_STRS;
    int value = menu_choice(CHOICE_TITLE, CHOICE_NUM, choices);
    //clear();
    //redrawGame();
    return value;
}
