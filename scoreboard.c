#include <curses.h>
#include <stdlib.h>

#include "scoreboard.h"

ScoreBoard* scoreboard_init() {
    ScoreBoard* board = malloc(sizeof(ScoreBoard));
    board->linesCleared = 0;
    board->score = 0;
    board->difficulty = 0;;

    return board;
}

void scoreboard_free(ScoreBoard* board) {
    if (board != NULL) {
        free(board);
    }
}

void scoreboard_clearLines(ScoreBoard* board, int n) {
    if (board != NULL) {
        board->linesCleared += n;
        int score;
        switch (n) {
            case 1: score = 40; break;
            case 2: score = 100; break;
            case 3: score = 300; break;
            case 4: score = 1200; break;
            default: score = 0;
        }
        board->score += score;
    }
}

void scoreboard_draw(ScoreBoard* board, WINDOW* win) {
    if (board == NULL || win == NULL) {
        return;
    }
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "Score:     %5d", board->score);
    mvwprintw(win, 2, 1, "Cleared:   %5d", board->score);
    mvwprintw(win, 3, 1, "Difficuly: %5d", board->score);
    touchwin(win);
    wrefresh(win);
}
