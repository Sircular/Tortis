/*
 * Tortis: A Tetris Clone
 * Walter Mays
 *
 * Entry Point
 */

#include <curses.h>
#include <time.h>
#include <stdbool.h>
#include <sys/select.h>
#include <sys/time.h>

#include "board.h"
#include "scoreboard.h"
#include "grabbag.h"
#include "color.h"
#include "menu.h"
#include "keys.h"

#define MICROS_TO_SECONDS 1000000

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 22
#define INFO_PANEL_WIDTH 8

#define GRABBAG_REPETITIONS 4

#define CHOICE_STRS {"Normal Mode", "Zen Mode", "Exit Tortis"}
#define CHOICE_NUM 3
#define CHOICE_TITLE "Main Menu"
#define CHOICE_PLAY 0
#define CHOICE_ZEN  1
#define CHOICE_EXIT 2

// TODO: Implement difficulty
#define TIMEOUT_COUNT 9
static long TIMEOUTS[] = {
    1000000,
    750000,
    500000,
    350000,
    250000,
    200000,
    150000,
    100000,
    50000
};

static Board* board;
static Board* previewBoard;
static ScoreBoard* scoreboard;

static WINDOW* gameWin;
static WINDOW* boardWin;
static WINDOW* previewWin;
static WINDOW* scoreWin;

static GrabBag* grabBag;

static bool isZen;

/* Performs curses and game struct initialization. */
void setup(void);

/* Initializes the game window and board window. */
void initWindows(void);

/* Redraws all of the windows and refreshes the screen. */
void redrawGame(void);

/* The main game loop. */
void gameLoop(void);

/* Gets input and simulates the block until it is locked into place. */
void dropBlock(long timeout);

/* Gets the next piece and updates the preview so that we can properly know
 * what pieces are coming. */
enum PieceType getNextPiece(void);

/* Redraws the window that shows the next piece. */
void redrawPreviewWindow(void);

/* Shows the main menu and returns the choice made. */
int showMainMenu(void);

int main() {
    setup();

    bool running = true;
    int choice;
    while (running) {
        // reset scores
        scoreboard->linesCleared = 0;
        scoreboard->score = 0;
        scoreboard->difficulty = 0;
        redrawGame();
        board_clear(board);

        choice = showMainMenu();
        endwin();
        switch (choice) {
            case CHOICE_PLAY:
                isZen = false;
                gameLoop();
                break;
            case CHOICE_ZEN:
                isZen = true;
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
    scoreboard_free(scoreboard);
    grabbag_free(grabBag);
    delwin(boardWin);
    delwin(gameWin);
    endwin();
    return 0;
}

void setup() {
    initscr();
    raw();
    noecho();
    curs_set(0);
    refresh();
    colors_init();
    board = board_init(BOARD_WIDTH, BOARD_HEIGHT);
    previewBoard = board_init(INFO_PANEL_WIDTH, INFO_PANEL_WIDTH);
    scoreboard = scoreboard_init();

    // initialize the grab bag for piece selection
    grabBag = grabbag_init(GRABBAG_REPETITIONS);
    getNextPiece();

    // initialize the windows
    initWindows(); 
}

void initWindows() {
    int boardWinWidth = (BOARD_WIDTH*2) + 2;
    int infoWinWidth = (INFO_PANEL_WIDTH*2) + 2;
    int totalWidth = boardWinWidth+infoWinWidth;
    int winHeight = BOARD_HEIGHT+2;
    int previewHeight = INFO_PANEL_WIDTH + 2;
    int infoWinHeight = winHeight - previewHeight;

    int tWidth, tHeight;
    getmaxyx(stdscr, tHeight, tWidth);

    gameWin = newwin(winHeight, totalWidth, (tHeight-winHeight)/2,
            (tWidth-totalWidth)/2);
    boardWin = derwin(gameWin, winHeight, boardWinWidth, 0, 0);
    previewWin = derwin(gameWin, previewHeight, infoWinWidth, 0, boardWinWidth);
    scoreWin = derwin(gameWin, infoWinHeight, infoWinWidth, previewHeight, boardWinWidth);
    redrawPreviewWindow();
}

void redrawGame() {
    board_draw(board, boardWin);
    wrefresh(gameWin);
    refresh();
}

void gameLoop() {
    redrawGame();
    redrawPreviewWindow();
    bool running = true;
    while (running) {
        if (board->piece == NULL) {
            enum PieceType piece = getNextPiece();
            board_setPiece(board, piece);
        }
        int timeoutIndex = (scoreboard->difficulty < TIMEOUT_COUNT) ? 
            scoreboard->difficulty : TIMEOUT_COUNT-1;
        long timeout = isZen ? TIMEOUTS[0] : TIMEOUTS[timeoutIndex];
        dropBlock(timeout);
        // check if you lost, bruh
        if (board->piece->pos.y <= 0) {
            // it's above the board
            running = false;
            break;
        }
        board_cementPiece(board);
        // check for any lines to clear
        int i;
        int turnLinesCleared = 0;
        for (i = 0; i < board->height; i++) {
            if (board_isLineFull(board, i)) {
                board_clearLine(board, i);
                board_shiftLine(board, i);
                turnLinesCleared++;
            }
        }
        scoreboard_clearLines(scoreboard, turnLinesCleared);
    }
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
                case KEY_QUICKDROP:
                    while (board_movePiece(board, 0, 1)){}
                    falling = false;
                    break;
                case KEY_PAUSE:
                    // simple pausing
                    while (getch() != KEY_PAUSE) {}
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

enum PieceType getNextPiece() {
    enum PieceType piece;
    if (previewBoard->piece != NULL) {
        piece = previewBoard->piece->type;
    } else {
        grabbag_next(grabBag, &piece);
    }
    enum PieceType newPiece;
    grabbag_next(grabBag, &newPiece);
    board_setPiece(previewBoard, newPiece);
    board_movePiece(previewBoard, 0, INFO_PANEL_WIDTH/2-1);
    //board->piece->pos = c;
    redrawPreviewWindow();
    return piece;
}

int showMainMenu() {
    char* choices[] = CHOICE_STRS;
    int value = menu_choice(CHOICE_TITLE, CHOICE_NUM, choices);
    return value;
}

void redrawPreviewWindow() {
    board_draw(previewBoard, previewWin);
    scoreboard_draw(scoreboard, scoreWin);
}
