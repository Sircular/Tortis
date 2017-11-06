/*
 * Defines the panel that keeps track of score, lines cleared, etc.
 */

#include <curses.h>

typedef struct ScoreBoard {
    int linesCleared;
    int difficulty;
    int score;
} ScoreBoard;

/* Creates a new scoreboard struct. */
ScoreBoard* scoreboard_init(void);

/* frees a scoreboard struct. */
void scoreboard_free(ScoreBoard* board);

/* Adds n lines to the count of lines cleared and increases the score
 * accordingly. */
void scoreboard_clearLines(ScoreBoard* board, int n);

/* Draws a scoreboard on a window. */
void scoreboard_draw(ScoreBoard* board, WINDOW* win);
