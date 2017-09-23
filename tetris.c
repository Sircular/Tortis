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

/* Redraws hello world on the screen */
void redraw(void);

static int i;

int main() {
    initscr();
    raw();
    noecho();
    nodelay(stdscr, true);

    // set up stuff for select
    fd_set in, tmp_in;
    struct timeval time;

    FD_ZERO(&in);
    FD_SET(0, &in);
    time.tv_sec = 0;
    time.tv_usec = 999999; // must be less than 1E+06

    i = 0;
    bool running = true;
    redraw();
    while (running) {
        tmp_in = in;
        int sel_ret = select(FD_SETSIZE, &tmp_in, NULL, NULL, &time);
        if (sel_ret == 0) { // timeout
            time.tv_sec = 0;
            time.tv_usec = 999999; // must be less than 1E+06
            redraw();
        } else { // keypress
            if (getch() == 10) {
                running = false;
            }
        }
    }

    endwin();
    return 0;
}

void redraw() {
    mvprintw(i, i, "Hello world!");
    refresh();
    i++;
}
