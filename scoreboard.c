#include <curses.h>
#include <stdlib.h>

#include "scoreboard.h"

void prettyPrint(int value, char* buffer);

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
        // check for difficulty
        board->difficulty = board->linesCleared/10;
    }
}

void scoreboard_draw(ScoreBoard* board, WINDOW* win) {
    if (board == NULL || win == NULL) {
        return;
    }
    box(win, 0, 0);
    char buffer[64];
    prettyPrint(board->score, buffer);
    mvwprintw(win, 1, 1, "Score:     %5s", buffer);
    mvwprintw(win, 2, 1, "Cleared:   %5d", board->linesCleared);
    mvwprintw(win, 3, 1, "Difficuly: %5d", board->difficulty);
    touchwin(win);
    wrefresh(win);
}

void prettyPrint(int value, char* buffer) {
    // figure out how many digits there are
    int digits = 0;
    int valCopy = value;
    while (valCopy > 0) {
        digits++;
        valCopy /= 10;
    }
    if (digits == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
    } else {
        int mask = 1;
        int i, bi;
        for (i = 0; i < digits; i++) {
            mask *= 10;
        }
        for (i = 0, bi = 0; i < digits; i++) {
            int digit = (value % mask) / (mask / 10);
            buffer[bi] = (char) ('0' + digit);
            bi++;
            mask /= 10;
            if ((digits - i - 1) % 3 == 0 && (digits - i - 1) > 0) {
                buffer[bi] = ',';
                bi++;
            }
        }
        buffer[bi] = '\0';
    }
}
