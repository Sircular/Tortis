#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <curses.h>

#include "menu.h"

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define CHOICE_STR "->"
#define CHOICE_STR_LEN (sizeof(CHOICE_STR)-1)
#define ENTER_CODE 10
#define UP_CODE 65
#define DOWN_CODE 66

/* Creates a window centered in the terminal. */
WINDOW* createCenteredWindow(int width, int height);

/* Draws the window, including all the choices and the current choice
 * indicator. */
void drawChoices(WINDOW* win, char* title, int choiceCount, int currentChoice,
        char** choices);

int menu_choice(char* title, int choiceCount, char** choices) {
    int winHeight, winWidth, i, choice, c;
    bool chosen;
    size_t longestChoice;
    WINDOW* menuWin;

    winHeight = choiceCount + 1 /* for the title bar */ +
        2 /* for the top and bottom bar */;

    longestChoice = 0;
    for (i = 0; i < choiceCount; i++) {
        size_t newLen = strlen(choices[i]);
        if (newLen > longestChoice) {
            longestChoice = newLen;
        }
    }
    winWidth = (int)(longestChoice + CHOICE_STR_LEN * 2 + 2);

    menuWin = createCenteredWindow(winHeight, winWidth);

    // drawing
    choice = 0;
    chosen = false;

    while (!chosen) {
        drawChoices(menuWin, title, choiceCount, choice, choices);
        c = getch();
        switch (c) {
            case ENTER_CODE:
                chosen = true;
                break;
            case UP_CODE:
                if (choice > 0) {
                    choice--;
                }
                break;
            case DOWN_CODE:
                if (choice < choiceCount-1) {
                    choice++;
                }
        }
    }
    free(menuWin);

    if (chosen) {
        return choice;
    } else {
        return -1;
    }
}

void menu_show(char* title, char* text) {
    int winHeight = 4;
    int winWidth = 2+(int)MAX(strlen(title), strlen(text));
    WINDOW* win = createCenteredWindow(winHeight, winWidth);

    box(win, 0, 0);
    mvwprintw(win, 1, (winWidth-(int)strlen(title))/2, title);
    mvwprintw(win, 2, (winWidth-(int)strlen(text))/2, text);

    wrefresh(win);
    refresh();
    getch();
    free(win);
}

WINDOW* createCenteredWindow(int height, int width) {
    int termWidth = 0, termHeight = 0;
    getmaxyx(stdscr, termHeight, termWidth);
    return newwin(height, width, (termHeight-height)/2, (termWidth-width)/2);
}

void drawChoices(WINDOW* win, char* title, int choiceCount, int currentChoice,
        char** choices) {
    int winWidth, winHeight, i;
    getmaxyx(win, winHeight, winWidth);
    wclear(win);
    box(win, 0, 0);
    mvwprintw(win, 1, (winWidth-(int)strlen(title))/2, title);
    for (i = 0; i < choiceCount; i++) {
        mvwprintw(win, i+2, CHOICE_STR_LEN+1, choices[i]);
        if (i == currentChoice) {
            mvwprintw(win, i+2, 1, CHOICE_STR);
        }
    }
    wrefresh(win);
    refresh();
}
