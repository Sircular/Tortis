/* Defines a simple coordinate struct that is used to define blocks and
 * positions in the tetris system. */

#ifndef TETRIS_COORDINATE
#define TETRIS_COORDINATE

typedef struct Coordinate {
    int x;
    int y;
} Coordinate;

/* Initializes a coordinate. */
#define COORDINATE(X, Y) { .x = X, .y = Y }

/* Returns the sum of two coordinates. Good for calculating offsets. */
Coordinate coordinate_add(Coordinate a, Coordinate b);

#endif
