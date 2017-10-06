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

#define SECONDS_TO_MILLIS 1000
#define MICROS_TO_MILLIS 1000

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#define GRABBAG_REPETITIONS 4
#define TIMEOUT 1000L // in milliseconds

#define MAX(x, y) ((x) < (y) ? (y) : (x))

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

/* Gets the difference between two timevals. */
long timeDifference(struct timeval* first, struct timeval* second);

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
    struct timeval current, last;
    bool running = true;
    long currentTimeout = TIMEOUT;
    gettimeofday(&last, NULL);

    redrawGame();

    while (running) {
        board_rotatePiece(board, false);
        redrawGame();
        timeout(currentTimeout);
        int c = getch();
        gettimeofday(&current, NULL);
        if (c == ERR) {
            // timeout
            currentTimeout = TIMEOUT;
            if (!board_movePiece(board, 0, 1)) {
                running = false;
            }
        } else {
            long elapsedTime = MAX(0, timeDifference(&last, &current));
            currentTimeout = MAX(0, currentTimeout-elapsedTime);
            // handle other keys
            if (c == 10) {
                running = false;
            }
        }
        last = current;
    }
    timeout(-1);
}

long timeDifference(struct timeval* first, struct timeval* second) {
    return (second->tv_sec - first->tv_sec) * SECONDS_TO_MILLIS + 
        (second->tv_usec - first->tv_usec) / MICROS_TO_MILLIS;
}

int showMainMenu() {
    char* choices[] = CHOICE_STRS;
    int value = menu_choice(CHOICE_TITLE, CHOICE_NUM, choices);
    //clear();
    //redrawGame();
    return value;
}
