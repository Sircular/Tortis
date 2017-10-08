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
#include "grabbag.h"
#include "color.h"
#include "menu.h"
#include "keys.h"

#define MICROS_TO_SECONDS 1000000

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 22
#define INFO_PANEL_WIDTH 8

#define GRABBAG_REPETITIONS 4

#define CHOICE_STRS {"Play", "Exit"}
#define CHOICE_NUM 2
#define CHOICE_TITLE "Main Menu"
#define CHOICE_PLAY 0
#define CHOICE_EXIT 1

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

static int difficulty;
static int linesCleared;
static int score;

static Board* board;
static Board* previewBoard;

static WINDOW* gameWin;
static WINDOW* boardWin;
static WINDOW* previewWin;
static WINDOW* infoWin;

static GrabBag* grabBag;

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
        linesCleared = 0;
        score = 0;
        difficulty = 0;
        redrawGame();
        board_clear(board);

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

void setup() {
    initscr();
    raw();
    noecho();
    curs_set(0);
    refresh();
    colors_init();
    board = board_init(BOARD_WIDTH, BOARD_HEIGHT);
    previewBoard = board_init(INFO_PANEL_WIDTH, INFO_PANEL_WIDTH);

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
    infoWin = derwin(gameWin, infoWinHeight, infoWinWidth, previewHeight, boardWinWidth);
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
        int timeoutIndex = (difficulty < TIMEOUT_COUNT) ? 
            difficulty : TIMEOUT_COUNT-1;
        dropBlock(TIMEOUTS[timeoutIndex]);
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
        linesCleared += turnLinesCleared;
        // increase the score
        switch(turnLinesCleared) {
            case 1:
                score += 40*(difficulty+1);
                break;
            case 2:
                score += 100*(difficulty+1);
                break;
            case 3:
                score += 300*(difficulty+1);
                break;
            case 4:
                score += 1200*(difficulty+1);
                break;
        }
        // recalculate difficulty
        difficulty = linesCleared/10;
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
    box(infoWin, 0, 0);
    mvwprintw(infoWin, 1, 1, "Score: %5d", score);
    mvwprintw(infoWin, 3, 1, "Lines: %5d", linesCleared);
    mvwprintw(infoWin, 5, 1, "Level: %5d", difficulty);
    wrefresh(infoWin);
}
